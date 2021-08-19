#pragma once

#include "IFramework.h"
#include "ThreadPool.h"

using namespace std;

class UsingThreadPool : IFrameWork
{
public:
	UsingThreadPool(int ThreadCount);
	~UsingThreadPool();

	void Receive(Task& _Task) override;

private:
	int m_ThreadCount;
	ThreadPool m_ThreadPool;
};
