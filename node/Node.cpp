#include <cassert>
#include <algorithm>

#include "base/Director.h"
#include "node/Node.h"
#include "node/Scene.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Camera.h"


int Node::_globalOrderOfArrival = 1;

Node::Node() :
	_parent(nullptr),
	_localZOrder(0),
	_globalZOrder(0),
	_orderOfArrival(0),
	_position(Vec3::ZERO),
	_rotation(Vec3::ZERO),
	_scale(Vec3::ONE),
	_rotationQuat(Quaternion::ZERO),
	_modelViewTransform(Mat4::IDENTITY),
	_transform(Mat4::IDENTITY),
	_transformDirty(true),
	_inverse(Mat4::IDENTITY),
	_inverseDirty(true),
	_transformUpdated(true),
	_reorderChildDirty(false),
	_contentSize(Size::ZERO),
	_contentSizeDirty(true),
	_tag(-1),
	_name(""),
	_hashOfName(0),
	_cameraMask(CameraFlag::DEFAULT),
	_isVisible(true),
	_shaderProgram(nullptr)
{
}


Node::~Node()
{
	if (_shaderProgram) 
	{
		_shaderProgram->release();
		_shaderProgram = nullptr;
	}
}



void Node::visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
	if (!_isVisible) return;

	uint32_t flags = processParentFlags(parentTransform, parentFlags);

	bool visibleByCamera = isVisitableByVisitingCamera();

	int i = 0;
	if (!_children.empty())
	{
		sortAllChildren();
		for (; i < _children.size(); ++i)
		{
			auto node = _children.at(i);
			if (node && node->_localZOrder < 0)
				node->visit(renderer, _modelViewTransform, flags);
			else
				break;
		}
		if (visibleByCamera) this->draw(renderer, _modelViewTransform, flags);

		for (auto it = _children.cbegin() + i; it != _children.cend(); ++it)
		{
			(*it)->visit(renderer, _modelViewTransform, flags);
		}
	}
	else if (visibleByCamera)
	{
		this->draw(renderer, _modelViewTransform, flags);
	}
}


void Node::draw()
{
	auto renderer = Director::getInstance()->getRenderer();
	draw(renderer, _modelViewTransform, true);
}


void Node::draw(Renderer * renderer, const Mat4 & parentTransform, uint32_t parentFlags)
{
}


void Node::setPosition(const Vec3 & pos)
{
	setPosition(pos.x, pos.y, pos.z);
}


void Node::setPosition(float x, float y, float z)
{
	if (_position.x == x && _position.y == y && _position.z == z)
		return;

	_position.x = x;
	_position.y = y;
	_position.z = z;

	_transformUpdated = _transformDirty = _inverseDirty = true;
}


void Node::setRotation(const Vec3 & rot)
{
	setRotation(rot.x, rot.y, rot.z);
}


void Node::setRotation(float x, float y, float z)
{
	if (_rotation.x == x && _rotation.y == y && _rotation.z == z)
		return;

	_rotation.x = x;
	_rotation.y = y;
	_rotation.z = z;

	_transformUpdated = _transformDirty = _inverseDirty = true;

	updateRotationQuat();
}


void Node::setScale(const Vec3 & scale)
{
	setScale(scale.x, scale.y, scale.z);
}


void Node::setScale(float x, float y, float z)
{
	if (_scale.x == x && _scale.y == y && _scale.z == z)
		return;

	_scale.x = x;
	_scale.y = y;
	_scale.z = z;

	_transformUpdated = _transformDirty = _inverseDirty = true;
}


void Node::setParent(Node * parent)
{
	_parent = parent;
	_transformUpdated = _transformDirty = _inverseDirty = true;
}


void Node::setName(const std::string& name)
{
	_name = name;
	std::hash<std::string> h;
	_hashOfName = h(name);
}


void Node::setContentSize(const Size & size)
{
	if (!size.equals(_contentSize))
	{
		_contentSize = size;
		_transformUpdated = _transformDirty = _inverseDirty = _contentSizeDirty = true;
	}
}


const Size & Node::getContentSize() const
{
	return _contentSize;
}


void Node::addChild(Node* child)
{
	assert(child, "Argument must be non-nil");
	this->addChild(child, child->_localZOrder, child->_name);
}


void Node::addChild(Node * child, int localZOrder)
{
	assert(child, "Argument must be non-nil");
	this->addChild(child, localZOrder, child->_name);
}


void Node::addChild(Node * child, int localZorder, int tag)
{
	assert(child, "Argument must be non-nil");
	assert(child->_parent == nullptr, "child already added. It can't be added again.");

	addChildHelper(child, localZorder, tag, "", true);
}


void Node::addChild(Node * child, int localZorder, const std::string & name)
{
	assert(child, "Argument must be non-nil");
	assert(child->_parent == nullptr, "child already added. It can't be added again.");

	addChildHelper(child, localZorder, INVALID_TAG, name, false);
}


void Node::sortAllChildren()
{
	if (_reorderChildDirty)
	{
		std::sort(std::begin(_children), std::end(_children), [](Node* n1, Node* n2) {
			return (n1->getLocalZOrder() < n2->getLocalZOrder() ||
				(n1->getLocalZOrder() == n2->getLocalZOrder() && n1->getOrderOfArrival() < n2->getOrderOfArrival()));
		});
		_reorderChildDirty = false;
	}
}


void Node::addChildHelper(Node * child, int localZorder, int tag, const std::string & name, bool setTag)
{
	if (_children.empty())
	{
		this->childrenAlloc();
	}

	this->insertChild(child, localZorder);

	if (setTag) child->setTag(tag);
	else child->setName(name);

	child->setParent(this);
	child->setOrderOfArrival(_globalOrderOfArrival++);

	child->onEnter();
}


const Mat4& Node::getNodeToParentTransform() const
{
	if (_transformDirty)
	{
		float x = _position.x;
		float y = _position.y;
		float z = _position.z;

		Mat4 translation;
		Mat4::createTranslation(x, y, z, &translation);
		Mat4::createRotation(_rotationQuat, &_transform);

		_transform = translation * _transform;
		if (_scale.x != 1.0f)
		{
			_transform.m[0] *= _scale.x, _transform.m[1] *= _scale.x, _transform.m[2] *= _scale.x;
		}
		if (_scale.y != 1.0f)
		{
			_transform.m[4] *= _scale.y, _transform.m[5] *= _scale.y, _transform.m[6] *= _scale.y;
		}
		if (_scale.z != 1.0f)
		{
			_transform.m[8] *= _scale.z, _transform.m[9] *= _scale.z, _transform.m[10] *= _scale.z;
		}

		_transformDirty = false;
	}

	return _transform;
}


Mat4 Node::getNodeToParentTransform(Node* ancestor) const
{
	Mat4 t(this->getNodeToParentTransform());

	for (Node* p = _parent; p && p != ancestor; p = p->getParent())
	{
		t = p->getNodeToParentTransform() * t;
	}

	return t;
}


Mat4 Node::getNodeToWorldTransform() const
{
	return this->getNodeToParentTransform(nullptr);
}


void Node::updateRotation()
{
	float x = _rotationQuat.x;
	float y = _rotationQuat.y;
	float z = _rotationQuat.z;
	float w = _rotationQuat.w;

	_rotation.x = atan2f(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));
	float sy = 2.0f * (w * y - z * x);
	sy = clampf(sy, -1, 1);
	_rotation.y = asinf(sy);
	_rotation.z = atan2f(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));

	_rotation.x = MATH_RAD_TO_DEG(_rotation.x);
	_rotation.y = MATH_RAD_TO_DEG(_rotation.y);
	_rotation.z = -MATH_RAD_TO_DEG(_rotation.z);
}


void Node::setRotationQuat(const Quaternion& quat)
{
	_rotationQuat = quat;
	updateRotation();
	_transformUpdated = _transformDirty = _inverseDirty = true;
}


Quaternion Node::getRotationQuat() const
{
	return _rotationQuat;
}


void Node::childrenAlloc()
{
	_children.reserve(4);
}


void Node::insertChild(Node * child, int z)
{
	_transformUpdated = true;
	_reorderChildDirty = true;
	_children.pushBack(child);
	child->_localZOrder = z;
}


void Node::detachChild(Node* child, size_t index, bool doCleanup)
{
	child->onExit();
	child->setParent(nullptr);
	if (doCleanup)
	{
		child->cleanup();
	}
	_children.erase(index);
}


Mat4 Node::transform(const Mat4& parentTransform)
{
	return parentTransform * this->getNodeToParentTransform();
}


uint32_t Node::processParentFlags(const Mat4& parentTransform, uint32_t parentFlags)
{
	auto flags = parentFlags;
	flags |= (_transformUpdated ? FLAGS_TRANSFORM_DIRTY : 0);
	flags |= (_contentSizeDirty ? FLAGS_CONTENT_SIZE_DIRTY : 0);
	
	if (flags & FLAGS_DIRTY_MASK)
		_modelViewTransform = this->transform(parentTransform);

	_transformUpdated = false;
	_contentSizeDirty = false;

	return flags;
}


void Node::updateRotationQuat()
{
	// convert Euler angle to quaternion
	float halfRadx = MATH_DEG_TO_RAD(_rotation.x / 2.0f);
	float halfRady = MATH_DEG_TO_RAD(_rotation.y / 2.0f);
	float halfRadz = -MATH_DEG_TO_RAD(_rotation.z / 2.0f);
	float coshalfRadx = cosf(halfRadx);
	float sinhalfRadx = sinf(halfRadx);
	float coshalfRady = cosf(halfRady);
	float sinhalfRady = sinf(halfRady);
	float coshalfRadz = cosf(halfRadz);
	float sinhalfRadz = sinf(halfRadz);

	_rotationQuat.x = sinhalfRadx * coshalfRady * coshalfRadz - coshalfRadx * sinhalfRady * sinhalfRadz;
	_rotationQuat.y = coshalfRadx * sinhalfRady * coshalfRadz + sinhalfRadx * coshalfRady * sinhalfRadz;
	_rotationQuat.z = coshalfRadx * coshalfRady * sinhalfRadz - sinhalfRadx * sinhalfRady * coshalfRadz;
	_rotationQuat.w = coshalfRadx * coshalfRady * coshalfRadz + sinhalfRadx * sinhalfRady * sinhalfRadz;
}


void Node::setOrderOfArrival(int orderOfArrival)
{
	assert(orderOfArrival >= 0, "Invalid orderOfArrival");
	_orderOfArrival = orderOfArrival;
}


void Node::setShaderProgram(ShaderProgram * program)
{
	_shaderProgram = program; 
	_shaderProgram->retain();
}


Scene* Node::getScene() const
{
	if (!_parent) return nullptr;

	auto sceneNode = _parent;
	while (sceneNode->_parent)
	{
		sceneNode = sceneNode->_parent;
	}

	return dynamic_cast<Scene*>(sceneNode);
}


void Node::onEnter()
{
	for (const auto& child : _children)
		child->onEnter();
}


void Node::onExit()
{
	for (const auto& child : _children)
		child->onExit();
}


void Node::update(float dt)
{
	for (const auto& child : _children)
		child->update(dt);
}


Node* Node::getChildByTag(int tag) const
{
	assert(tag != Node::INVALID_TAG, "Invalid tag");

	for (const auto& child : _children)
	{
		if (child && child->_tag == tag)
			return child;
	}
	return nullptr;
}


Node* Node::getChildByName(const std::string & name)
{
	assert(!name.empty(), "Invalid name");
	
	std::hash<std::string> h;
	size_t hash = h(name);

	for (const auto& child : _children)
	{
		if (child->_hashOfName == hash && child->_name.compare(name) == 0)
			return child;
	}
	return nullptr;
}


void Node::removeChild(Node * child, bool cleanup)
{
	if (_children.empty())
		return;
	size_t index = _children.getIndex(child);
	if (index != -1)
		this->detachChild(child, index, cleanup);
}


void Node::removeChildByTag(int tag, bool cleanup)
{
	assert(tag != Node::INVALID_TAG, "invalid tag");
	Node* child = this->getChildByTag(tag);
	assert(child, "child not found!");

	this->removeChild(child, cleanup);
}


void Node::removeFromParentAndCleanup(bool cleanup)
{
	assert(_parent, "parent not found!");
	_parent->removeChild(this, cleanup);
}


void Node::removeFromParent()
{
	removeFromParentAndCleanup(true);
}


bool Node::isVisitableByVisitingCamera() const
{
	auto camera = Camera::getVisitingCamera();
	bool visibleByCamera = camera ? (camera->getCameraFlag() & _cameraMask) != 0 : true;
	return visibleByCamera;
}


void Node::removeAllChildren()
{
	this->removeAllChildrenWithCleanup(true);
}


void Node::removeAllChildrenWithCleanup(bool cleanup)
{
	for (const auto& child : _children)
	{
		child->onExit();

		if (cleanup) child->cleanup();
		child->setParent(nullptr);
	}
	_children.clear();
}
