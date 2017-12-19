#pragma once

#include <string>
#include <unordered_map>

class Texture2D;

class TextureCache
{

public:
	
	TextureCache(const TextureCache&) = delete;
	virtual ~TextureCache() = default;
	
	static TextureCache& getInstance();

	Texture2D* addImage(const std::string& filePath);

private:

	TextureCache() = default;

private:

	std::unordered_map<std::string, Texture2D*> _textures;

};