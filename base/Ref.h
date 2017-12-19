#pragma once

#include <iostream>
using std::cout;
using std::endl;

class Ref
{

public:

	Ref();
	virtual ~Ref();

	void retain();
	void release();
	void autorelease();

private:

	unsigned int _referenceCount;

};

