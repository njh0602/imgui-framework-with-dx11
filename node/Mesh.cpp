#include <new>
#include <iostream>
using namespace std;

#include "base/Director.h"
#include "base/MeshLoader.h"
#include "renderer/Renderer.h"
#include "node/Mesh.h"
#include "node/Model3D.h"

Mesh::Mesh() :
	_owner(nullptr),
	_name(""),
	_primitiveType("TRIANGLES"),
	_vertices(nullptr),
	_indices(nullptr),
	_numOfVertices(0),
	_numOfIndices(0)
{
}


Mesh::~Mesh()
{
	delete[] _vertices;
	_vertices = nullptr;

	delete[] _indices;
	_indices = nullptr;
}


Mesh* Mesh::createWithFile(const std::string & fileName)
{
	auto ret = new (std::nothrow) Mesh();
	if (ret && ret->initWithFile(fileName))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	ret = nullptr;

	return nullptr;
}


bool Mesh::initWithFile(const std::string & fileName)
{
	_name = fileName;

	if (0 == MeshLoader::loadMesh(fileName, *this))
		return true;

	assert(false, "error load mesh");

	return false;
}