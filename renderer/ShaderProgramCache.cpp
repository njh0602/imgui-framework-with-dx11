#include "renderer/ShaderProgramCache.h"
#include "renderer/ShaderProgram.h"
#include "renderer/DefaultShader.h"


ShaderProgramCache::ShaderProgramCache()
{
}


ShaderProgramCache::~ShaderProgramCache()
{
	for (auto& shader : _programs)
	{
		shader.second->release();
		shader.second = nullptr;
	}
	_programs.clear();
}


ShaderProgramCache& ShaderProgramCache::getInstance()
{
	static ShaderProgramCache instance;
	return instance;
}


ShaderProgram* ShaderProgramCache::getShaderProgram(const std::string& key) const
{
	auto it = _programs.find(key);
	if (it != std::end(_programs))
		return it->second;
	return nullptr;
}


void ShaderProgramCache::addShaderProgram(const std::string& key, ShaderProgram * program)
{
	// release old one
	auto prev = getShaderProgram(key);
	if (prev == program)
		return;

	_programs.erase(key);
	if (prev) 
	{
		prev->release();
		prev = nullptr;
	}

	if (program) program->retain();
	_programs[key] = program;
}
