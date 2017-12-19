#include "renderer/TextureCache.h"
#include "renderer/Texture2D.h"
#include "renderer/Image.h"

#include <D3DX11.h>


TextureCache& TextureCache::getInstance()
{
	static TextureCache instance;
	return instance;
}


Texture2D* TextureCache::addImage(const std::string & filePath)
{
	if (filePath.empty())
		return nullptr;

	Texture2D* texture = nullptr;
	Image* image = nullptr;
	auto iter = _textures.find(filePath);
	if (iter != std::end(_textures))
		texture = iter->second;

	if (!texture)
	{
		do 
		{
			image = new (std::nothrow) Image();
			if (!image) break;

			// only png
			auto ret = image->initWithPngFile(filePath);
			if (!ret) break;

			texture = new (std::nothrow) Texture2D();
			if (texture && texture->initWithImage(image))
			{
				_textures.insert({ filePath, texture });
			}
			else
			{
				texture = nullptr;
			}

		} while (0);
	}

	return texture;
}
