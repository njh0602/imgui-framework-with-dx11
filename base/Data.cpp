#include "base/Data.h"


Data::Data() : 
	_bytes(nullptr),
	_size(0)
{
}


Data::Data(const Data & rhs) :
	_bytes(nullptr),
	_size(0)
{
	copy(rhs._bytes, rhs._size);
}


Data::Data(Data && rhs) :
	_bytes(nullptr),
	_size(0)
{
	move(rhs);
}


Data::~Data()
{
	clear();
}


Data& Data::operator=(const Data & rhs)
{
	copy(rhs._bytes, rhs._size);
	return *this;
}


Data& Data::operator=(Data && rhs)
{
	move(rhs);
	return *this;
}


unsigned char * Data::getBytes() const
{
	return _bytes;
}


size_t Data::getSize() const
{
	return _size;
}


void Data::copy(const unsigned char * bytes, const size_t size)
{
	clear();

	if (size > 0)
	{
		_size = size;
		_bytes = new unsigned char[_size];
		memcpy(_bytes, bytes, size);
	}
}


void Data::clear()
{
	delete[] _bytes;
	_bytes = nullptr;
	_size = 0;
}

bool Data::isNull() const
{
	return (_bytes == nullptr || _size == 0);
}

void Data::move(Data& rhs)
{
	clear();

	_bytes = rhs._bytes;
	_size = rhs._size;

	rhs._bytes = nullptr;
	rhs._size = 0;
}
