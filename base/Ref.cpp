#include <cassert>

#include "base/Ref.h"
#include "base/AutoreleasePool.h"

Ref::Ref() :
	_referenceCount(1)
{
}


Ref::~Ref()
{
}


void Ref::retain()
{
	_referenceCount++;
}


void Ref::release()
{
	assert(_referenceCount > 0, "can not _referenceCount <= 0");

	_referenceCount--;
	if (_referenceCount == 0)
	{
		delete this;
	}
}


void Ref::autorelease()
{
	AutoreleasePool::getInstance().addObject(this);
}
