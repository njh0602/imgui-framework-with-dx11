#pragma once

#include <string>

#include "base/Ref.h"
#include "base/Vector.h"
#include "math/Vec2.h"
#include "math/Vec3.h"
#include "math/Mat4.h"
#include "math/Quaternion.h"
#include "math/Geometry.h"


class Scene;
class ShaderProgram;
class Renderer;

class Node : public Ref
{

public:

	static const int INVALID_TAG = -1;

	enum 
	{
		FLAGS_TRANSFORM_DIRTY = (1 << 0),
		FLAGS_CONTENT_SIZE_DIRTY = (1 << 1),
		FLAGS_RENDER_AS_3D = (1 << 3),
		FLAGS_DIRTY_MASK = (FLAGS_TRANSFORM_DIRTY | FLAGS_CONTENT_SIZE_DIRTY),
	};

public:

	Node();
	virtual ~Node();
	virtual bool init() { return true; }

	virtual void visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags);

	virtual void draw() final;
	virtual void draw(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags);

	virtual void setPosition(const Vec3& pos);
	virtual void setPosition(float x, float y, float z);
	virtual const Vec3 getPosition() const { return _position; }

	virtual void setRotation(const Vec3& rot);
	virtual void setRotation(float x, float y, float z);
	virtual const Vec3 getRotation() const { return _rotation; }

	virtual void setScale(const Vec3& scale);
	virtual void setScale(float x, float y, float z);
	virtual const Vec3 getScale() const { return _scale; }

	void setParent(Node* parent);
	Node* getParent() const { return _parent; }

	void setTag(int tag) { _tag = tag; }
	int getTag() const { return _tag; }

	int getLocalZOrder() const { return _localZOrder; }
	void setLocalZOrder(int zOrder) { _localZOrder = zOrder; }

	void setName(const std::string& name);
	std::string getName() const { return _name; }

	void setContentSize(const Size& size);
	const Size& getContentSize() const;

	virtual void addChild(Node* child);
	virtual void addChild(Node* child, int localZOrder);
	virtual void addChild(Node* child, int localZorder, int tag);
	virtual void addChild(Node* child, int localZorder, const std::string& name);

	void sortAllChildren();

	void setOrderOfArrival(int orderOfArrival);
	int getOrderOfArrival() const { return _orderOfArrival; }

	void setShaderProgram(ShaderProgram* program);
	ShaderProgram* getShaderProgram() const { return _shaderProgram; }

	Scene* getScene() const;

	virtual void onEnter();
	virtual void onExit();
	virtual void update(float dt);

	Node* getChildByTag(int tag) const;
	Node* getChildByName(const std::string& name);

	virtual void removeChild(Node* child, bool cleanup);
	virtual void removeChildByTag(int tag, bool cleanup);
	virtual void removeAllChildren();
	virtual void removeAllChildrenWithCleanup(bool cleanup);

	virtual void removeFromParentAndCleanup(bool cleanup);
	virtual void removeFromParent();

	void cleanup() {}

	bool isVisitableByVisitingCamera() const;

	bool isVisible() const { return _isVisible; }
	void setVisible(bool enable) { _isVisible = enable; }

	const Mat4& getNodeToParentTransform() const;
	Mat4 getNodeToParentTransform(Node* ancestor) const;
	Mat4 getNodeToWorldTransform() const;

	void updateRotation();

	void setRotationQuat(const Quaternion& quat);
	Quaternion getRotationQuat() const;

protected:

	void childrenAlloc();
	void insertChild(Node* child, int z);
	void detachChild(Node* child, size_t index, bool doCleanup);

	Mat4 transform(const Mat4 &parentTransform);
	uint32_t processParentFlags(const Mat4& parentTransform, uint32_t parentFlags);

	void updateRotationQuat();

private:

	void addChildHelper(Node* child, int localZorder, int tag, const std::string& name, bool setTag);

protected:

	Node*					_parent;
	Vector<Node*>			_children;

	int						_localZOrder;
	int						_globalZOrder;
	int						_orderOfArrival;
	static int				_globalOrderOfArrival;

	
	Vec3					_position;
	Vec3					_rotation;
	Vec3					_scale;
	Quaternion				_rotationQuat;

	Mat4					_modelViewTransform;

	mutable Mat4			_transform;
	mutable bool			_transformDirty;
	mutable Mat4			_inverse;
	mutable bool			_inverseDirty;
	mutable bool			_transformUpdated;
	bool					_reorderChildDirty;

	Size					_contentSize;
	bool					_contentSizeDirty;

	int						_tag;
	std::string				_name;
	size_t					_hashOfName;

	unsigned short			_cameraMask;

	bool					_isVisible;

	ShaderProgram*			_shaderProgram;

};

