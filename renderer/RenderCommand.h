#pragma once

#include "math/Mat4.h"

class Mesh;

class RenderCommand
{

	friend class Renderer;

public:

	enum class Type
	{
		UNKNOWN_COMMAND,
		TRIANGLE_COMMAND,
		CUSTOM_COMMAND
	};

public:

	RenderCommand();
	virtual ~RenderCommand();

	bool init(float globalZOrder, const Mat4& transform, uint32_t flags);

	Type getType() const { return _type; }

	float getGlobalZOrder() const { return _globalZOrder; }

	bool is3D() const { return _is3D; }
	void set3D(bool value) { _is3D = value; }

	float getDepth() const { return _depth; }

protected:

	Type		_type;

	float		_globalZOrder;
	bool		_is3D;
	float		_depth;

	Mat4		_transform;

};

