#pragma once

#include <functional>
#include <vector>


class Scheduler
{

public:

	Scheduler();
	virtual ~Scheduler();

	void update(float dt);
	void addCallback(const std::function<void(float)>& callback);

private:



private:

	std::vector<std::function<void(float)>> _callbacks;

};