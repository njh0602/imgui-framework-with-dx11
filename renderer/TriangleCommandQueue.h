#pragma once

#include <vector>


class TriangleCommand;

class TriangleCommandQueue
{

public:

	TriangleCommandQueue();
	~TriangleCommandQueue();

	const std::vector<TriangleCommand*>& getCommands() const { return _commands; }

	void addCommand(TriangleCommand* command);

	int getTotalVertexSize() const { return _totalVertexSize; }
	int getTotalIndexSize() const { return _totalIndexSize; }

	void clear();

private:

	std::vector<TriangleCommand*> _commands;
	int _totalVertexSize;
	int _totalIndexSize;

};