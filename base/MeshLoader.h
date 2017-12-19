#pragma once

#include <string>

#include "tinyxml2/tinyxml2.h"

class Mesh;

class MeshLoader
{

public:

	MeshLoader() = default;
	virtual ~MeshLoader() = default;

	static tinyxml2::XMLError loadMesh(const std::string& fileName, Mesh& dst);

};