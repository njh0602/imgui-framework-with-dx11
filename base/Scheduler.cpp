#include "Scheduler.h"


Scheduler::Scheduler()
{
}


Scheduler::~Scheduler()
{
}


void Scheduler::update(float dt)
{
	for (const auto& callback : _callbacks)
		callback(dt);
}


void Scheduler::addCallback(const std::function<void(float)>& callback)
{
	_callbacks.push_back(callback);
}
