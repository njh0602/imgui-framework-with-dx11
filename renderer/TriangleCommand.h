#pragma once

#include "renderer/RenderCommand.h"
#include "renderer/TriangleCommandQueue.h"
#include "renderer/Types.h"


class ShaderProgram;

class TriangleCommand : public RenderCommand
{

	friend class Renderer;
	friend class TriangleCommandQueue;

public:

	TriangleCommand();
	virtual ~TriangleCommand();

	bool init(float globalZOrder, 
		VertexBase* vertices, int numOfVertices, IndexBuffer* indices, int numOfIndices,
		ShaderProgram* program, const Mat4& transform, uint32_t flags);

private:

	VertexBase*		_vertices;
	int				_numOfVertices;
	IndexBuffer*	_indices;
	int				_numOfIndices;
	ShaderProgram*  _shaderProgram;
	
};
