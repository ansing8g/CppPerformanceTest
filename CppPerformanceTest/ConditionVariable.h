#pragma once

#include "IFramework.h"

using namespace std;

class ConditionVariable : IFrameWork
{
private:
	static mutex m_MutexLog;

private:
	void Worker(int _Index);

public:
	ConditionVariable(int ThreadCount);
	~ConditionVariable();

	void Receive(Task& _Task) override;

	bool PopTask(Task& _pTask);
	bool IsEmptyTask();

private:
	int m_ThreadCount;
	vector<thread> m_vecThread;
	bool m_run;
	Concurrency::concurrent_queue<Task> m_TaskQueue;
	mutex m_Mutex;
	condition_variable m_ConditionVariable;
};
