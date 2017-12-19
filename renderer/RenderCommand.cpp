#include "renderer/RenderCommand.h"
#include "node/Node.h"
#include "node/Mesh.h"

RenderCommand::RenderCommand() :
	_type(Type::UNKNOWN_COMMAND),
	_globalZOrder(0.0f),
	_is3D(false),
	_depth(0.0f),
	_transform(Mat4::IDENTITY)
{
}


RenderCommand::~RenderCommand()
{
}


bool RenderCommand::init(float globalZOrder, const Mat4 & transform, uint32_t flags)
{
	_globalZOrder = globalZOrder;
	_transform = transform;

	if (flags & Node::FLAGS_RENDER_AS_3D)
	{
		set3D(true);
	}
	else
	{
		set3D(false);
		_depth = 0.0f;
	}

	return true;
}
