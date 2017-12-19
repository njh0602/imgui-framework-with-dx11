#pragma once

#include <functional>

#include "renderer/RenderCommand.h"
#include "renderer/Types.h"


class CustomCommand : public RenderCommand
{

public:

	CustomCommand();
	virtual ~CustomCommand();

	bool init(float globalZOrder);
	void execute();

	std::function<void()> func;

};