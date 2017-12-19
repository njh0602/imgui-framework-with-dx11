#include "renderer/TriangleCommand.h"


TriangleCommand::TriangleCommand() : RenderCommand()
{
	_type = Type::TRIANGLE_COMMAND;
}


TriangleCommand::~TriangleCommand()
{
}


bool TriangleCommand::init(float globalZOrder,
	VertexBase* vertices,
	int numOfVertices, 
	IndexBuffer* indices, 
	int numOfIndices,
	ShaderProgram* program,
	const Mat4 & transform, uint32_t flags)
{
	if (!RenderCommand::init(globalZOrder, transform, flags))
		return false;

	_vertices = vertices;
	_numOfVertices = numOfVertices;
	_indices = indices;
	_numOfIndices = numOfIndices;
	_shaderProgram = program;

	return true;
}
