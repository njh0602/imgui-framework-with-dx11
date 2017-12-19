#pragma once

#include <string>


class Image;

class Texture2D
{

public:

	bool initWithImage(Image* image);

private:

	std::string _filePath;

};