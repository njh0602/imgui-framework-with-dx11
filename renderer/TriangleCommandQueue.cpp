#include "TriangleCommandQueue.h"
#include "TriangleCommand.h"


TriangleCommandQueue::TriangleCommandQueue() :
	_commands(),
	_totalVertexSize(0),
	_totalIndexSize(0)
{
}


TriangleCommandQueue::~TriangleCommandQueue()
{
}


void TriangleCommandQueue::addCommand(TriangleCommand* command)
{
	_commands.push_back(command); 
	_totalVertexSize += command->_numOfVertices * sizeof(V3F_C4F);
	_totalIndexSize += command->_numOfIndices * sizeof(IndexBuffer);
}


void TriangleCommandQueue::clear()
{
	_commands.clear();
	_totalVertexSize = 0;
	_totalIndexSize = 0;
}
