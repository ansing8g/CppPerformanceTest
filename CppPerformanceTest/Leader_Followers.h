#pragma once

#include "IFramework.h"
#include "Task.h"

using namespace std;

class Leader_Followers : IFrameWork
{
private:
	void Worker(int index);

public:
	Leader_Followers(int ThreadCount);
	~Leader_Followers();

	void Receive(Task& _Task) override;

	bool PopTask(Task& _pTask);
	bool IsEmptyTask();

private:
	static atomic<bool> m_isExecute;

private:
	int m_ThreadCount;
	vector<thread> m_vecThread;
	bool m_run;
	Concurrency::concurrent_queue<Task> m_TaskQueue;
	mutex m_Mutex;
	condition_variable m_ConditionVariable;
};
