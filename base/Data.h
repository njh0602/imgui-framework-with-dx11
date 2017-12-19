#pragma once

#include <cstring>
#include <cstdlib>

class Data final
{

public:

	Data();
	Data(const Data& rhs);
	Data(Data&& rhs);
	~Data();

	Data& operator=(const Data& rhs);
	Data& operator=(Data&& rhs);

	unsigned char* getBytes() const;
	size_t getSize() const;

	void copy(const unsigned char* bytes, const size_t size);
	void clear();
	bool isNull() const;

private:

	void move(Data& rhs);

private:

	unsigned char* _bytes;
	size_t _size;

};