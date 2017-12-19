#include <new>

#include "base/Director.h"
#include "renderer/Renderer.h"
#include "renderer/DefaultShader.h"
#include "renderer/ShaderProgramCache.h"


DefaultShader::DefaultShader(ID3D11Device* device) : ShaderProgram(device)
{
}


DefaultShader::~DefaultShader()
{
}


DefaultShader* DefaultShader::createWithFileName(const std::string& fileName)
{
	auto cache = ShaderProgramCache::getInstance();
	auto program = static_cast<DefaultShader*>(cache.getShaderProgram(fileName));
	if (!program)
	{
		program = new (std::nothrow) DefaultShader(Director::getInstance()->getRenderer()->getDevice());
		if (program && program->initWithFile(fileName))
		{
			cache.addShaderProgram(fileName, program);
			program->autorelease();
			return program;
		}
		delete program;
		program = nullptr;
		return nullptr;
	}

	return program;
}


bool DefaultShader::initWithFile(const std::string & fileName)
{
	if (!ShaderProgram::initWithFile(fileName))
		return false;

	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;

	std::wstring wFileName(_fileName.begin(), _fileName.end());

	// compile vs
	HRESULT result = D3DX11CompileFromFile(wFileName.c_str(), NULL, NULL, "VShader", "vs_4_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		assert(errorMessage, (const char*)(errorMessage->GetBufferPointer()));
		assert(!errorMessage, "missing shader file.");
	}

	// create vs
	result = _device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), 
		NULL, &_vertexShader);

	assert(result == S_OK, "can not create vs");

	// create input layout
	D3D11_INPUT_ELEMENT_DESC desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	int numOfElements = sizeof(desc) / sizeof(desc[0]);

	result = _device->CreateInputLayout(desc, numOfElements,
		vertexShaderBuffer->GetBufferPointer(), 
		vertexShaderBuffer->GetBufferSize(),
		&_inputLayout);

	assert(result == S_OK, "can not create input layout");

	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;


	ID3D10Blob* pixelShaderBuffer;

	// compile ps
	result = D3DX11CompileFromFile(wFileName.c_str(),
		NULL, NULL,
		"PShader", "ps_4_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		assert(errorMessage, (const char*)(errorMessage->GetBufferPointer()));
		assert(!errorMessage, "missing shader file.");
	}

	// create ps
	result = _device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(),
		NULL, &_pixelShader);

	assert(result == S_OK, "can not create ps");

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	return true;
}
