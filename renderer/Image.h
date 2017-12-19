#pragma once

#include <string>

#include "base/Data.h"

class Image
{

public:

	enum class Format
	{
		PNG,
		UNKNOWN
	};

public:

	Image();
	virtual ~Image();

	bool initWithPngFile(const std::string & path);

	Format getFileType() const { return _fileType; }
	size_t getDataLen() const { return _dataLen; }
	int getWidth() const { return _width; }
	int getHeight() const { return _height; }
	unsigned char* getData() const { return _data; }
	std::string getFilePath() const { return _filePath; }

protected:

	Image(const Image&) = delete;
	Image& operator=(const Image&) = delete;

private:

	Format _fileType;
	std::string _filePath;
	size_t _dataLen;
	int _width;
	int _height;
	unsigned char* _data;

};