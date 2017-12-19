#pragma once

#include <chrono>
#include <vector>
#include <stack>

#include "base/Ref.h"
#include "base/Vector.h"
#include "math/Geometry.h"
#include "math/Mat4.h"


class Renderer;
class Scene;

class Director : public Ref
{

	friend class Application;
	friend class Renderer;

public:

	Director(const Director&) = delete;
	Director& operator=(const Director&) = delete;

	virtual ~Director();
	static Director* getInstance();

	virtual bool init();
	void mainLoop();
	
	Renderer* getRenderer() { return _renderer; }

	Size getWindowSize() const { return _windowSize; }

	void onResize(unsigned int width, unsigned int height);
	void pushScene(Scene* scene);

	void runWithScene(Scene* scene);

	const float getDeltaTime() const { return _deltaTime; }
	Scene* getRunningScene() const { return _runningScene; }

private:

	Director();
	static Director* create();
	
	void drawScene();
	void calculateDeltaTime();

private:

	static Director* _sharedInstance;

	Scene* _runningScene;
	Renderer* _renderer;

	Vector<Scene*> _sceneStack;

	float _deltaTime;
	std::chrono::high_resolution_clock::time_point _lastUpdate;

	Size _windowSize;

};

