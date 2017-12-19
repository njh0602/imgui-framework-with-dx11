#include "base/MeshCache.h"
#include "node/Mesh.h"


MeshCache::~MeshCache()
{
	for (auto& mesh : _meshs)
	{
		mesh.second->release();
		mesh.second = nullptr;
	}
	_meshs.clear();
}


MeshCache::MeshCache()
{
}


MeshCache & MeshCache::getInstance()
{
	static MeshCache instance;
	return instance;
}


Mesh* MeshCache::addMesh(const std::string& fileName)
{
	Mesh* mesh = nullptr;
	auto iter = _meshs.find(fileName);
	if (iter == std::end(_meshs))
	{
		mesh = Mesh::createWithFile(fileName);
		_meshs.insert({ fileName, mesh });
		mesh->retain();
	}
	else
	{
		mesh = iter->second;
	}

	return mesh;
}
