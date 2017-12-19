#include "base/MeshCache.h"
#include "renderer/Renderer.h"
#include "node/Model3D.h"
#include "node/Mesh.h"


Model3D::Model3D() : 
	_mesh(nullptr)
{
}


Model3D::~Model3D()
{
	_mesh = nullptr;
}


bool Model3D::initWithFile(const std::string& filePath)
{
	if (!Node::init())
		return false;

	auto mesh = MeshCache::getInstance().addMesh(filePath);
	if (!mesh) return false;
	setMesh(mesh);

	return true;
}


Model3D * Model3D::createWithFile(const std::string & filePath)
{
	auto ret = new (std::nothrow) Model3D();
	if (ret && ret->initWithFile(filePath))
	{
		ret->autorelease();
		return ret;
	}

	delete ret;
	ret = nullptr;

	return nullptr;
}


void Model3D::draw(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
	_triangleCommand.init(0.0f,
		_mesh->getVertices(), _mesh->getNumOfVertice(),
		_mesh->getIndices(), _mesh->getNumOfIndices(),
		_shaderProgram, parentTransform, 0);

	renderer->addCommand(&_triangleCommand);
}


void Model3D::setMesh(Mesh* mesh)
{
	_mesh = mesh;
	_mesh->setOwner(this);
}
