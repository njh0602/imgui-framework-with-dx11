#include <iostream>
#include <new>
#include <cassert>

#include "base/Application.h"
#include "base/Director.h"
#include "renderer/Renderer.h"
#include "scenes/HelloWorldScene.h"
#include "math/Geometry.h"

static HWND						__hwnd = 0;
static HINSTANCE				__hinstance = 0;

static const Size				WINDOW_SIZE = Size(1280, 768);


extern LRESULT ImGui_ImplDX11_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplDX11_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
		case WM_SIZE:
		{
			if (wParam != SIZE_MINIMIZED)
			{
				Director::getInstance()->onResize(static_cast<unsigned int>(LOWORD(lParam)), static_cast<unsigned int>(HIWORD(lParam)));
			}
			return 0;
		}
		case WM_SYSCOMMAND:
		{
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}	
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


Application::Application() :
	_director(nullptr),
	_renderer(nullptr)
{
}


Application::~Application()
{
	delete _director;
	_director = nullptr;

	if (__hwnd)
	{
		DestroyWindow(__hwnd);
		__hwnd = 0;
	}

	UnregisterClass(_applicationName, __hinstance);
	__hinstance = NULL;
}


bool Application::applicationDidFinishLaunching()
{
	bool result = createWindow(L"Learn DX11", Rect(0, 0, WINDOW_SIZE.width, WINDOW_SIZE.height));
	assert(result, "can not create window");

	_director = Director::getInstance();
	_director->_windowSize = WINDOW_SIZE;

	_renderer = Renderer::create(__hwnd, WINDOW_SIZE);
	_director->_renderer = _renderer;
	
	assert(_renderer, "can not create renderer");
	assert(_director, "can not create director");

	_renderer->retain();
	_director->retain();

	// regist start scene
	auto scene = HelloWorldScene::createScene();
	_director->runWithScene(scene);

	return true;
}


int Application::enterMessagePump()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		// game loop
		if ( _director ) _director->mainLoop();
	}

	return 1;
}


int Application::run()
{
	if (!applicationDidFinishLaunching())
		return 0;

	return enterMessagePump();
}

bool Application::createWindow(WCHAR* viewName, const Rect& rect)
{
	_applicationName = viewName;

	__hinstance = ::GetModuleHandle(NULL);

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = __hinstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = viewName;

	if (!RegisterClassEx(&wc)) return false;

	int posX = (GetSystemMetrics(SM_CXSCREEN) - rect.size.width) / 2;
	int posY = (GetSystemMetrics(SM_CYSCREEN) - rect.size.height) / 2;

	__hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		viewName,
		viewName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, 
		rect.size.width, rect.size.height, NULL, NULL,
		__hinstance, NULL);

	// Show the window
	ShowWindow(__hwnd, SW_SHOW);
	SetForegroundWindow(__hwnd);
	SetFocus(__hwnd);

	return true;
}
 