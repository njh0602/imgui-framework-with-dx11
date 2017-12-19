#include <new>
#include <iostream>
#include <wchar.h>
using std::cout;
using std::endl;

#include "base/Director.h"
#include "node/Scene.h"
#include "renderer/Renderer.h"
#include "renderer/RenderCommand.h"
#include "renderer/TriangleCommand.h"
#include "renderer/CustomCommand.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Camera.h"
#include "node/Mesh.h"


Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}


void Renderer::cleanupRenderTarget()
{
	if (_rasterizeState)
	{
		_rasterizeState->Release();
		_rasterizeState = 0;
	}

	if (_depthStencilView)
	{
		_depthStencilView->Release();
		_depthStencilView = 0;
	}

	if (_depthStencilState)
	{
		_depthStencilState->Release();
		_depthStencilState = 0;
	}

	if (_depthStencilBuffer)
	{
		_depthStencilBuffer->Release();
		_depthStencilBuffer = 0;
	}

	if (_mainRenderTargetView)
	{
		_mainRenderTargetView->Release();
		_mainRenderTargetView = 0;
	}
}


void Renderer::cleanupDeviceD3D()
{
	cleanupRenderTarget();
	if (_swapChain) { _swapChain->Release(); _swapChain = NULL; }
	if (_d3dDeviceContext) { _d3dDeviceContext->Release(); _d3dDeviceContext = NULL; }
	if (_d3dDevice) { _d3dDevice->Release(); _d3dDevice = NULL; }
	if (_vertexBuffer) { _vertexBuffer->Release(); _vertexBuffer = NULL; }
	if (_indexBuffer) { _indexBuffer->Release(); _indexBuffer = NULL; }
	if (_vertexConstantBuffer) { _vertexConstantBuffer->Release(); _vertexConstantBuffer = NULL; }
}


Renderer* Renderer::create(HWND hwnd, const Size& size)
{
	auto ret = new (std::nothrow) Renderer();
	if (ret && ret->init(hwnd, size))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	ret = nullptr;

	return nullptr;
}


bool Renderer::init(HWND hwnd, const Size& size)
{
	_hwnd = hwnd;
	_winSize = size;

	if (!initializeD3D11(hwnd)) return false;

	return true;
}


void Renderer::clear()
{
	float clearCol[4] = { 0, 0, 0, 1 };
	_d3dDeviceContext->ClearRenderTargetView(_mainRenderTargetView, clearCol);
	_d3dDeviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 0.0f, 0);

	clearRenderCommands();
}


void Renderer::render()
{
	_isRendering = true;

	auto runningScene = Director::getInstance()->_runningScene;
	auto winSize = Director::getInstance()->_windowSize;

	auto left = runningScene->getPosition().x;
	auto top = -runningScene->getPosition().y;
	auto right = left + runningScene->getClipSize().width;
	auto bottom = top + runningScene->getClipSize().height;

	D3D11_RECT r = {
		static_cast<LONG>(left),
		static_cast<LONG>(top),
		static_cast<LONG>(right),
		static_cast<LONG>(bottom)
	};
	_d3dDeviceContext->RSSetScissorRects(1, &r);
	processRenderCommands();

	_swapChain->Present(0, 0);

	_isRendering = false;
}


void Renderer::onResize(unsigned int width, unsigned int height)
{
	cleanupRenderTarget();
	_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	createRenderTargetDepthStencilView();
}


void Renderer::addCommand(RenderCommand* command)
{
	switch (command->getType())
	{
		case RenderCommand::Type::TRIANGLE_COMMAND:
		{
			_triangleCommands.addCommand(static_cast<TriangleCommand*>(command));
	
			break;
		}

		case RenderCommand::Type::CUSTOM_COMMAND:
		{
			_customCommands.push_back(static_cast<CustomCommand*>(command));

			break;
		}

		default: break;
	}
}


void Renderer::processRenderCommands()
{
	// process triangle commands
	_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if ( !_vertexBuffer || _vertexBufferSize < _triangleCommands.getTotalVertexSize() )
	{
		if (_vertexBuffer) 
		{
			_vertexBuffer->Release(); 
			_vertexBuffer = NULL;
		}

		_vertexBufferSize = _triangleCommands.getTotalVertexSize();

		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = _vertexBufferSize + 1;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		HRESULT isCreatedBuffer = _d3dDevice->CreateBuffer(&vertexBufferDesc, NULL, &_vertexBuffer);
		if (FAILED(isCreatedBuffer))
			throw std::runtime_error("vertex buffer cannot create");
	}

	if (!_indexBuffer || _indexBufferSize < _triangleCommands.getTotalIndexSize())
	{
		if (_indexBuffer) 
		{
			_indexBuffer->Release(); 
			_indexBuffer = NULL; 
		}

		_indexBufferSize = _triangleCommands.getTotalIndexSize();

		D3D11_BUFFER_DESC indexBufferDesc;
		indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		indexBufferDesc.ByteWidth = _indexBufferSize + 1;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		HRESULT isCreatedBuffer = _d3dDevice->CreateBuffer(&indexBufferDesc, NULL, &_indexBuffer);
		if (FAILED(isCreatedBuffer))
			throw std::runtime_error("index buffer cannot create");
	}

	if (_indexBuffer && _vertexBuffer)
	{
		// fill vertx, index buffer
		D3D11_MAPPED_SUBRESOURCE vtxResource, idxResource;
		if (_d3dDeviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtxResource) != S_OK) throw std::runtime_error("can not Map() vertex buffer");
		if (_d3dDeviceContext->Map(_indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &idxResource) != S_OK) throw std::runtime_error("can not Map() index buffer");

		V3F_C4F* vtxDst = (V3F_C4F*)vtxResource.pData;
		IndexBuffer* idxDst = (IndexBuffer*)idxResource.pData;
		for (const auto& triCommand : _triangleCommands.getCommands())
		{
			memcpy(vtxDst, triCommand->_vertices, triCommand->_numOfVertices * sizeof(V3F_C4F));
			memcpy(idxDst, triCommand->_indices, triCommand->_numOfIndices * sizeof(IndexBuffer));

			vtxDst += triCommand->_numOfVertices;
			idxDst += triCommand->_numOfIndices;
		}

		_d3dDeviceContext->Unmap(_vertexBuffer, 0);
		_d3dDeviceContext->Unmap(_indexBuffer, 0);

		unsigned int stride, offset;
		stride = sizeof(V3F_C4F);
		offset = 0;

		// set vertex, index buffer
		_d3dDeviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
		_d3dDeviceContext->IASetIndexBuffer(_indexBuffer, sizeof(IndexBuffer) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);

		int vtxOffset = 0;
		int idxOffset = 0;
		for (const auto& triCommand : _triangleCommands.getCommands())
		{
			// set constant buffer
			auto model = triCommand->_transform;
			auto view = Camera::getVisitingCamera()->getViewMatrix();
			auto projection = Camera::getVisitingCamera()->getProjectionMatrix();

			model.transpose();
			view.transpose();
			projection.transpose();

			D3D11_MAPPED_SUBRESOURCE ms;
			_d3dDeviceContext->Map(_vertexConstantBuffer, NULL, D3D11_MAP_WRITE_DISCARD, 0, &ms);

			_vertexConstantBufferData = static_cast<VertexConstantBuffer*>(ms.pData);
			_vertexConstantBufferData->mvp.mat1 = model;
			_vertexConstantBufferData->mvp.mat2 = view;
			_vertexConstantBufferData->mvp.mat3 = projection;

			_d3dDeviceContext->Unmap(_vertexConstantBuffer, 0);

			_d3dDeviceContext->VSSetConstantBuffers(0, 1, &_vertexConstantBuffer);
			_d3dDeviceContext->IASetInputLayout(triCommand->_shaderProgram->_inputLayout);
			_d3dDeviceContext->VSSetShader(triCommand->_shaderProgram->_vertexShader, NULL, 0);
			_d3dDeviceContext->PSSetShader(triCommand->_shaderProgram->_pixelShader, NULL, 0);

			// draw
			_d3dDeviceContext->DrawIndexed(triCommand->_numOfIndices, idxOffset, vtxOffset);

			idxOffset += triCommand->_numOfIndices;
			vtxOffset += triCommand->_numOfVertices;
		}
	}

	// process custom commands
	for (const auto& customCommand : _customCommands)
	{
		customCommand->execute();
	}
}


void Renderer::clearRenderCommands()
{
	_triangleCommands.clear();
	_customCommands.clear();
}


bool Renderer::initializeD3D11(HWND hwnd)
{
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width = _winSize.width;
	bufferDesc.Height = _winSize.height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE,
		NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&_swapChain,
		&_d3dDevice,
		NULL,
		&_d3dDeviceContext);

	createRenderTargetDepthStencilView();

	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth = sizeof(VertexConstantBuffer);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	HRESULT isCreatedBuffer = _d3dDevice->CreateBuffer(&constantBufferDesc, NULL, &_vertexConstantBuffer);
	if (FAILED(isCreatedBuffer)) throw std::runtime_error("constant buffer cannot create");

	return true;
}


void Renderer::createRenderTargetDepthStencilView()
{
	ID3D11Texture2D *pBackBuffer;
	_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	_d3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &_mainRenderTargetView);
	pBackBuffer->Release();

	D3D11_TEXTURE2D_DESC deptBufferDesc;
	deptBufferDesc.Width = _winSize.width;
	deptBufferDesc.Height = _winSize.height;
	deptBufferDesc.MipLevels = 1;
	deptBufferDesc.ArraySize = 1;
	deptBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	deptBufferDesc.SampleDesc.Count = 1;
	deptBufferDesc.SampleDesc.Quality = 0;
	deptBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	deptBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	deptBufferDesc.CPUAccessFlags = 0;
	deptBufferDesc.MiscFlags = 0;

	_d3dDevice->CreateTexture2D(&deptBufferDesc, NULL, &_depthStencilBuffer);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	_d3dDevice->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
	_d3dDeviceContext->OMSetDepthStencilState(_depthStencilState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	_d3dDevice->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
	_d3dDeviceContext->OMSetRenderTargets(1, &_mainRenderTargetView, _depthStencilView);

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = true;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	
	_d3dDevice->CreateRasterizerState(&rasterDesc, &_rasterizeState);
	_d3dDeviceContext->RSSetState(_rasterizeState);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.Width = _winSize.width;
	viewport.Height = _winSize.height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	_d3dDeviceContext->RSSetViewports(1, &viewport);
}