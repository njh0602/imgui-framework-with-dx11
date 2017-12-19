#pragma once


#include <string>
#include <unordered_map>


class ShaderProgram;

class ShaderProgramCache
{

public:

	virtual ~ShaderProgramCache();

	static ShaderProgramCache& getInstance();

	ShaderProgram* getShaderProgram(const std::string& key) const;
	void addShaderProgram(const std::string& key, ShaderProgram* program);

private:

	ShaderProgramCache();

private:

	std::unordered_map<std::string, ShaderProgram*> _programs;

};