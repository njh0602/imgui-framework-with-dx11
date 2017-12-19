#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <tchar.h>
#include <wchar.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <DxErr.h>

#include <vector>
#include <string>

#include "base/Ref.h"
#include "math/Geometry.h"
#include "math/Mat4.h"
#include "renderer/Types.h"
#include "renderer/TriangleCommandQueue.h"


class RenderCommand;
class CustomCommand;
class TriangleCommand;
class ShaderProgram;

struct VertexConstantBuffer
{
	M4F_M4F_M4F mvp;
};

class Renderer : public Ref
{

public:

	Renderer();
	virtual ~Renderer();

	static Renderer* create(HWND hwnd, const Size& size);
	virtual bool init(HWND hwnd, const Size& size);

	void render();
	void clear();
	void onResize(unsigned int width, unsigned int height);

	void addCommand(RenderCommand* command);
	void clearRenderCommands();
	void processRenderCommands();

	ID3D11Device* getDevice() const { return _d3dDevice; }
	ID3D11DeviceContext* getDeviceContext() const { return _d3dDeviceContext; }
	HWND getWindowHandle() const { return _hwnd; }

private:

	bool initializeD3D11(HWND hwnd);
	void cleanupDeviceD3D();
	void cleanupRenderTarget();
	void createRenderTargetDepthStencilView();

private:

	Size						_winSize;
	HWND						_hwnd;
	ID3D11Device*				_d3dDevice = 0;
	ID3D11DeviceContext*		_d3dDeviceContext = 0;
	IDXGISwapChain*				_swapChain = 0;
	ID3D11RenderTargetView*		_mainRenderTargetView = 0;
	ID3D11Texture2D*			_depthStencilBuffer = 0;
	ID3D11DepthStencilState*	_depthStencilState = 0;
	ID3D11DepthStencilView*		_depthStencilView = 0;
	ID3D11RasterizerState*		_rasterizeState = 0;

	ID3D11Buffer*				_vertexBuffer = 0;
	ID3D11Buffer*				_indexBuffer = 0;

	ID3D11Buffer*				_vertexConstantBuffer = 0;
	VertexConstantBuffer*		_vertexConstantBufferData = nullptr;

	TriangleCommandQueue		_triangleCommands;
	int							_vertexBufferSize = 0;
	int							_indexBufferSize = 0;

	std::vector<CustomCommand*> _customCommands;

	bool						_isRendering = false;

};

