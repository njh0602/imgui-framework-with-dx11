#include <new>

#include "node/Scene.h"
#include "renderer/Camera.h"

Camera* Camera::_visitingCamera = nullptr;

Camera::Camera() :
	_scene(nullptr),
	_fieldOfView(0.0f),
	_acpectRatio(0.0f),
	_nearPlane(0.0f),
	_farPlane(0.0f),
	_position(Vec3::ZERO),
	_rotation(Vec3::ZERO),
	_projection(Mat4::IDENTITY),
	_view(Mat4::IDENTITY),
	_viewInv(Mat4::IDENTITY),
	_viewProjection(Mat4::IDENTITY),
	_viewProjectionDirty(false),
	_viewProjectionUpdated(false),
	_cameraFlag(CameraFlag::DEFAULT)
{
}


Camera::~Camera()
{
}


Camera * Camera::createPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	auto ret = new (std::nothrow) Camera();
	if (ret && ret->initWithPerspective(fieldOfView, aspectRatio, nearPlane, farPlane))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	ret = nullptr;

	return nullptr;
}

bool Camera::initWithPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	if (!Node::init())
		return false;

	_fieldOfView = fieldOfView;
	_acpectRatio = aspectRatio;
	_nearPlane = nearPlane;
	_farPlane = farPlane;

	Mat4::createPerspective(_fieldOfView, _acpectRatio, _nearPlane, _farPlane, &_projection);

	_viewProjectionDirty = true;

	return true;
}


void Camera::setScene(Scene* scene)
{
	if (_scene != scene)
	{
		// remove old scene
		if (_scene)
		{
			auto& cameras = _scene->_cameras;
			auto it = std::find(cameras.begin(), cameras.end(), this);
			if (it != cameras.end())
				cameras.erase(it);
			_scene = nullptr;
		}
		//set new scene
		if (scene)
		{
			_scene = scene;
			auto& cameras = _scene->_cameras;
			auto it = std::find(cameras.begin(), cameras.end(), this);
			if (it == cameras.end())
			{
				_scene->_cameras.pushBack(this);
			}
		}
	}
}


void Camera::visit(Renderer * renderer, const Mat4 & parentTransform, uint32_t parentFlags)
{
	_viewProjectionUpdated = _transformUpdated;
	Node::visit(renderer, parentTransform, parentFlags);
}


void Camera::onEnter()
{
	if ( !_scene )
	{
		auto scene = getScene();
		if ( scene )
		{
			setScene(scene);
		}
	}

	Node::onEnter();
}


void Camera::onExit()
{
	// remove this camera from scene
	setScene(nullptr);
	Node::onExit();
}


void Camera::apply()
{
}


void Camera::lookAt(const Vec3 & target, const Vec3 & up)
{
	Mat4::createLookAt(getPosition(), target, up, &_view);
}


const Mat4& Camera::getProjectionMatrix() const
{
	return _projection;
}


const Mat4& Camera::getViewMatrix() const
{
	Mat4 viewInv(getNodeToWorldTransform());
	static int count = sizeof(float) * 16;
	if (std::memcmp(viewInv.m, _viewInv.m, count) != 0)
	{
		_viewProjectionDirty = true;
		_viewInv = viewInv;
		_view = viewInv.getInversed();
	}
	return _view;
}


const Mat4& Camera::getViewProjectionMatrix() const
{
	_view = getViewMatrix();
	if ( _viewProjectionDirty )
	{
		_viewProjectionDirty = false;
		Mat4::multiply(_projection, _view, &_viewProjection);
	}

	return _viewProjection;
}