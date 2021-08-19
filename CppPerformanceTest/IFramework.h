#pragma once

class Task;

class IFrameWork
{
public:
	virtual void Receive(Task& _Task) = 0;
};
