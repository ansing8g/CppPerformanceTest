#include "stdafx.h"
#include "Task.h"
#include "Server.h"
#include "ConditionVariable.h"

//--> static private
mutex ConditionVariable::m_MutexLog;

//--> private
void ConditionVariable::Worker(int _Index)
{
	Server::m_ThreadCount.fetch_add(1);

	while (this->m_run)
	{
		Server::m_ThreadCount.fetch_sub(1);

		unique_lock<mutex> Lock(this->m_Mutex);
		this->m_ConditionVariable.wait(Lock, [&]()->bool { return !this->IsEmptyTask(); });
		Lock.unlock();

		Server::m_ThreadCount.fetch_add(1);

		Task _Task;
		if (false == this->PopTask(_Task))
		{
			continue;
		}

		while (e_JobType::None != _Task.NextJobType())
		{
			_Task.Execute(_Index);
		}
	}
}

//--> public
ConditionVariable::ConditionVariable(int ThreadCount)
	: IFrameWork()
	, m_ThreadCount(ThreadCount)
	, m_vecThread()
	, m_run(true)
	, m_TaskQueue()
	, m_Mutex()
{
	for (int i = 0; i < m_ThreadCount; ++i)
	{
		m_vecThread.push_back(thread(&ConditionVariable::Worker, this, i + 1));
	}
}

ConditionVariable::~ConditionVariable()
{
	m_run = false;

	for (size_t i = 0; i < m_vecThread.size(); ++i)
	{
		m_vecThread[i].join();
	}
	m_vecThread.clear();

	m_TaskQueue.clear();
}

void ConditionVariable::Receive(Task& _Task)
{
	m_TaskQueue.push(_Task);
	
	m_ConditionVariable.notify_one();
}

bool ConditionVariable::PopTask(Task& _pTask)
{
	return m_TaskQueue.try_pop(_pTask);
}

bool ConditionVariable::IsEmptyTask()
{
	return m_TaskQueue.empty();
}
