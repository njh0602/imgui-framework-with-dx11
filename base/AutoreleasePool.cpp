#include "base/AutoreleasePool.h"
#include "base/Ref.h"

AutoreleasePool::AutoreleasePool()
{
}


AutoreleasePool::~AutoreleasePool()
{
}

AutoreleasePool& AutoreleasePool::getInstance()
{
	static AutoreleasePool instance;
	return instance;
}


void AutoreleasePool::addObject(Ref * object)
{
	_managedObjects.push_back(object);
}


void AutoreleasePool::clear()
{
	std::vector<Ref*> releasings;
	releasings.swap(_managedObjects);
	for (const auto& obj : releasings)
		obj->release();
}
