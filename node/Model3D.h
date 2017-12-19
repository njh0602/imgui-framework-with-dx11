#pragma once

#include <string>

#include "node/Node.h"
#include "renderer/TriangleCommand.h"

class Mesh;
class Renderer;

class Model3D : public Node
{

	friend class Mesh;

public:

	Model3D();
	virtual ~Model3D();

	static Model3D* createWithFile(const std::string& filePath);
	virtual bool initWithFile(const std::string& filePath);

	virtual void draw(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags) override;

	void setMesh(Mesh* mesh);

private:

	Mesh* _mesh;
	TriangleCommand	_triangleCommand;

};

