#pragma once

#include "renderer/ShaderProgram.h"


class TexturedShader : public ShaderProgram
{

public:

	explicit TexturedShader(ID3D11Device* device);
	virtual ~TexturedShader();

	static TexturedShader* createWithFileName(const std::string& fileName);
	virtual bool initWithFile(const std::string& fileName) override;

private:

	ID3D11ShaderResourceView* _texture;
	ID3D11SamplerState* _samplerState;

};