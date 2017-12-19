#pragma once

#include <unordered_map>
#include <string>


class Mesh;

class MeshCache
{

public:

	virtual ~MeshCache();

	static MeshCache& getInstance();
	Mesh* addMesh(const std::string& fileName);

private:

	MeshCache();

private:

	std::unordered_map<std::string, Mesh*> _meshs;

};