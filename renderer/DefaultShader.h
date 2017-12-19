#pragma once

#include "renderer/ShaderProgram.h"


class DefaultShader : public ShaderProgram
{

public:

	explicit DefaultShader(ID3D11Device* device);
	virtual ~DefaultShader();

	static DefaultShader* createWithFileName(const std::string& fileName);
	virtual bool initWithFile(const std::string& fileName) override;

};