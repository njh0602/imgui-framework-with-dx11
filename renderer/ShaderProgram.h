#pragma once

#include <string>
#include <unordered_map>

#include <d3d11.h>
#include <d3dx11.h>

#include "base/Ref.h"
#include "math/Mat4.h"
#include "renderer/Types.h"


class ShaderProgram : public Ref
{

	friend class Renderer;

public:

	explicit ShaderProgram(ID3D11Device* device);
	virtual ~ShaderProgram();

	virtual bool initWithFile(const std::string& fileName);

	std::string getFileName() const { return _fileName; }

protected:

	ID3D11Device*			_device;
	std::string				_fileName;

	ID3D11VertexShader*		_vertexShader;
	ID3D11PixelShader*		_pixelShader;
	ID3D11InputLayout*		_inputLayout;

};


