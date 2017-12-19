#include "CustomCommand.h"


CustomCommand::CustomCommand()
{
	_type = Type::CUSTOM_COMMAND;
}


CustomCommand::~CustomCommand()
{
}


bool CustomCommand::init(float globalZOrder)
{
	_globalZOrder = globalZOrder;

	return true;
}


void CustomCommand::execute()
{
	if (func)
	{
		func();
	}
}
