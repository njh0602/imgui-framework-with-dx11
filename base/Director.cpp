#include <new>
#include <chrono>
#include <iostream>
#include <cassert>

#include "base/Director.h"
#include "base/AutoreleasePool.h"
#include "renderer/Renderer.h"
#include "node/Scene.h"

Director* Director::_sharedInstance = nullptr;

Director::Director() :
	_runningScene(nullptr),
	_renderer(nullptr),
	_deltaTime(0.0f),
	_lastUpdate(std::chrono::high_resolution_clock::now())
{
}


Director * Director::create()
{
	auto ret = new (std::nothrow) Director();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	ret = nullptr;

	return nullptr;
}


void Director::drawScene()
{
	assert(_renderer, "the renderer should not null");
	assert(_runningScene, "the runningScene should not null");

	calculateDeltaTime();

	_runningScene->update(_deltaTime);

	_renderer->clear();
	_runningScene->render(_renderer);
}


void Director::calculateDeltaTime()
{
	auto now = std::chrono::high_resolution_clock::now();
	_deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - _lastUpdate).count() / 1000000.0f;
	_deltaTime = max(0.0f, _deltaTime);

	// prevent big delta time
	if (_deltaTime > 0.2f)
	{
		_deltaTime = 1 / 60.0f;
	}

	_lastUpdate = now;
}


Director::~Director()
{
}


Director* Director::getInstance()
{
	if (!_sharedInstance)
	{
		_sharedInstance = Director::create();
	}
	return _sharedInstance;
}


bool Director::init()
{	
	return true;
}

void Director::mainLoop()
{
	drawScene();

	// release the objects
	AutoreleasePool::getInstance().clear();
}


void Director::onResize(unsigned int width, unsigned int height)
{
	if ( _renderer ) _renderer->onResize(width, height);
}


void Director::pushScene(Scene* scene)
{
	assert(scene, "the scene should not null");

	_sceneStack.pushBack(scene);
	_runningScene = scene;
}


void Director::runWithScene(Scene* scene)
{
	assert(scene != nullptr, "This command can only be used to start the Director. There is already a scene present.");
	assert(_runningScene == nullptr, "_runningScene should be null");

	pushScene(scene);
}