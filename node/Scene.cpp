#include <new>

#include "base/Director.h"
#include "node/Scene.h"
#include "renderer/Camera.h"
#include "renderer/Renderer.h"


Scene::Scene() : 
	_defaultCamera(nullptr)
{
}


Scene::~Scene()
{
}


Scene* Scene::create()
{
	auto ret = new (std::nothrow) Scene();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	ret = nullptr;

	return nullptr;
}


bool Scene::init()
{
	if (!Node::init())
		return false;

	auto winSize = Director::getInstance()->getWindowSize();
	setClipSize(winSize);

	_defaultCamera = Camera::createPerspective(60.0f, winSize.width / winSize.height, 1.0f, 3000.0f);
	_defaultCamera->setPosition(Vec3(0.0f, 0.0f, 800.0f));
	_defaultCamera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3::UNIT_Y);
	addChild(_defaultCamera);

	return true;
}


void Scene::render(Renderer* renderer)
{
	auto director = Director::getInstance();
	const auto& transform = getNodeToParentTransform();

	for (const auto& camera : _cameras)
	{
		if (!camera->isVisible()) continue;
		Camera::_visitingCamera = camera;
		this->visit(renderer, transform, 0);

		renderer->render();
	}

	Camera::_visitingCamera = nullptr;
}


void Scene::removeAllChildren()
{
	if (_defaultCamera)
		_defaultCamera->retain();

	Node::removeAllChildren();
	
	if (_defaultCamera)
	{
		addChild(_defaultCamera);
		_defaultCamera->release();
	}
}

void Scene::setClipSize(const Size & size)
{
	_clipSize = size;
}
