#pragma once

#include "IFramework.h"

using namespace std;

class LockControl : IFrameWork
{
private:
	static void Worker(int index, LockControl* _LockControl);
	static mutex m_MutexLog;

public:
	LockControl(int _ThreadCount);
	~LockControl();

	virtual void Receive(Task& _Task) override;

	bool PopTask(Task& _pTask);

private:
	vector<thread> m_vecThread;
	int m_ThreadCount;
	bool m_run;
	Concurrency::concurrent_queue<Task> m_TaskQueue;
	mutex m_ProcessLock;
};
