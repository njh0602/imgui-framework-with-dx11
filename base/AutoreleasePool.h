#pragma once

#include <vector>

class Ref;

class AutoreleasePool
{

public:

	virtual ~AutoreleasePool();

	static AutoreleasePool& getInstance();

	void addObject(Ref* object);
	void clear();

private:

	AutoreleasePool();

private:

	std::vector<Ref*> _managedObjects;

};

