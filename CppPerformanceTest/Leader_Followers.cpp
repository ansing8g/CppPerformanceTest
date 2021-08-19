#include "stdafx.h"
#include "Server.h"
#include "Logger.h"
#include "Leader_Followers.h"

atomic<bool> Leader_Followers::m_isExecute = true;

//--> static private
void Leader_Followers::Worker(int _Index)
{
	Server::m_ThreadCount.fetch_add(1);

	while (this->m_run)
	{
		Server::m_ThreadCount.fetch_sub(1);

		unique_lock<mutex> Lock(this->m_Mutex);
		this->m_ConditionVariable.wait(Lock, [&]()->bool { return !this->IsEmptyTask(); });

		Server::m_ThreadCount.fetch_add(1);

		Task _Task;
		if (false == this->PopTask(_Task))
		{
			Lock.unlock();
			continue;
		}

		Lock.unlock();

		while (e_JobType::None != _Task.NextJobType())
		{
			_Task.Execute(_Index);
		}
	}

	//bool ExcuteState = true, WaiteState = false;
	//
	//if (nullptr == _Leader_Followers)
	//{
	//	return;
	//}
	//
	//while (_Leader_Followers->m_run)
	//{
	//	while(false == Leader_Followers::m_isExecute.compare_exchange_weak(ExcuteState, WaiteState))
	//	{
	//		this_thread::sleep_for(chrono::milliseconds(1));
	//		ExcuteState = true;
	//		WaiteState = false;
	//	}
	//	
	//	Task _Task;
	//	bool isGetTask = _Leader_Followers->PopTask(_Task);
	//
	//	Leader_Followers::m_isExecute.store(ExcuteState);
	//
	//	if (false == isGetTask)
	//	{
	//		continue;
	//	}
	//
	//	while (e_JobType::None != _Task.NextJobType())
	//	{
	//		_Task.Execute(index);
	//	}
	//}
}

//--> public
Leader_Followers::Leader_Followers(int ThreadCount)
	: IFrameWork()
	, m_ThreadCount(ThreadCount)
	, m_vecThread()
	, m_run(true)
	, m_TaskQueue()
	, m_Mutex()
	, m_ConditionVariable()
{
	for (int i = 0; i < m_ThreadCount; ++i)
	{
		m_vecThread.push_back(thread(&Leader_Followers::Worker, this, i + 1));
	}
}

Leader_Followers::~Leader_Followers()
{
	m_run = false;

	for (size_t i = 0; i < m_vecThread.size(); ++i)
	{
		m_vecThread[i].join();
	}
	m_vecThread.clear();

	m_TaskQueue.clear();
}

void Leader_Followers::Receive(Task& _Task)
{
	m_TaskQueue.push(_Task);

	m_ConditionVariable.notify_one();
}

bool Leader_Followers::PopTask(Task& _pTask)
{
	return m_TaskQueue.try_pop(_pTask);
}

bool Leader_Followers::IsEmptyTask()
{
	return m_TaskQueue.empty();
}
