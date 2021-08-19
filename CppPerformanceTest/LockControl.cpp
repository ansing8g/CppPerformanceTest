#include "stdafx.h"
#include "Task.h"
#include "Server.h"
#include "LockControl.h"
#include "Logger.h"

//--> static private
void LockControl::Worker(int index, LockControl* _LockControl)
{
	if (nullptr == _LockControl)
	{
		return;
	}

	Server::m_ThreadCount.fetch_add(1);

	while (_LockControl->m_run)
	{
		Server::m_ThreadCount.fetch_sub(1);

		Task _Task;

		_LockControl->m_ProcessLock.lock();

		while (false == _LockControl->PopTask(_Task))
		{
			this_thread::sleep_for(chrono::milliseconds(1));
			continue;
		}

		_LockControl->m_ProcessLock.unlock();

		Server::m_ThreadCount.fetch_add(1);

		while (e_JobType::None != _Task.NextJobType())
		{
			_Task.Execute(index);
		}
	}
}

mutex LockControl::m_MutexLog;

//--> public
LockControl::LockControl(int _ThreadCount)
	: IFrameWork()
	, m_vecThread()
	, m_ThreadCount(_ThreadCount)
	, m_run(true)
	, m_TaskQueue()
	, m_ProcessLock()
{
	for (int i = 0; i < m_ThreadCount; ++i)
	{
		m_vecThread.push_back(thread(&LockControl::Worker, i+1, this));
	}
}

LockControl::~LockControl()
{
	m_run = false;

	for (size_t i = 0; i < m_vecThread.size(); ++i)
	{
		m_vecThread[i].join();
	}
	m_vecThread.clear();

	m_TaskQueue.clear();
}

void LockControl::Receive(Task& _Task)
{
	m_TaskQueue.push(_Task);
}

bool LockControl::PopTask(Task& _pTask)
{
	return m_TaskQueue.try_pop(_pTask);
}
