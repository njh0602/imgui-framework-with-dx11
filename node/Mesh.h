#pragma once

#include <string>

#include "base/Ref.h"
#include "node/Node.h"
#include "math/Vec3.h"
#include "math/Vec4.h"
#include "renderer/TriangleCommand.h"
#include "renderer/Types.h"

class Node;
class Model3D;

class Mesh : public Ref
{

	friend class MeshLoader;
	friend class Renderer;

public:

	Mesh();
	virtual ~Mesh();

	static Mesh* createWithFile(const std::string& fileName);
	virtual bool initWithFile(const std::string& fileName);

	void setOwner(Model3D* owner) { _owner = owner; }

	std::string getName() const { return _name; }

	VertexBase* getVertices() const { return _vertices; }
	IndexBuffer* getIndices() const { return _indices; }

	int getNumOfVertice() const { return _numOfVertices; }
	int getNumOfIndices() const { return _numOfIndices; }

protected:

	Model3D*			_owner;

	std::string			_name;
	std::string 		_primitiveType;

	VertexBase*			_vertices;
	IndexBuffer*		_indices;

	int					_numOfVertices;
	int					_numOfIndices;

};