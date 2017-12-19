#include "renderer/Texture2D.h"
#include "renderer/Image.h"
#include "math/Geometry.h"

bool Texture2D::initWithImage(Image* image)
{
	if (!image) return false;

	_filePath = image->getFilePath();

	unsigned char* tempData = image->getData();
	int width = image->getWidth();
	int height = image->getHeight();
	size_t dataLen = image->getDataLen();
	Size imageSize = Size(width, height);

	// 

	return true;
}
