#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "math/Geometry.h"

class Director;
class Renderer;

class Application
{

public:

	Application();
	virtual ~Application();

	int run();
	
private:

	bool createWindow(WCHAR* viewName, const Rect& rect);
	bool applicationDidFinishLaunching();
	int enterMessagePump();

private:

	WCHAR* _applicationName;
	Director* _director;
	Renderer* _renderer;

};