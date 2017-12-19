#include <new>
#include <cassert>

#include "base/Director.h"
#include "renderer/ShaderProgram.h"
#include "renderer/ShaderProgramCache.h"
#include "renderer/Renderer.h"


ShaderProgram::ShaderProgram(ID3D11Device* device) :
	_device(device),
	_fileName(""),
	_vertexShader(0),
	_pixelShader(0),
	_inputLayout(0)
{
}


ShaderProgram::~ShaderProgram()
{
	if (_inputLayout)
	{
		_inputLayout->Release();
		_inputLayout = 0;
	}

	if (_pixelShader)
	{
		_pixelShader->Release();
		_pixelShader = 0;
	}

	if (_vertexShader)
	{
		_vertexShader->Release();
		_vertexShader = 0;
	}
}


bool ShaderProgram::initWithFile(const std::string& fileName)
{
	_fileName = fileName;

	return true;
}