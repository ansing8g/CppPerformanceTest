#include "stdafx.h"
#include "Task.h"
#include "Server.h"
#include "UsingThreadPool.h"

UsingThreadPool::UsingThreadPool(int ThreadCount)
	: m_ThreadCount(ThreadCount)
	, m_ThreadPool(ThreadCount)
{

}

UsingThreadPool::~UsingThreadPool()
{

}

void UsingThreadPool::Receive(Task& _Task)
{
	m_ThreadPool.enqueue([_Task]()mutable->void {
		Server::m_ThreadCount.fetch_add(1);

		while (e_JobType::None != _Task.NextJobType())
		{
			_Task.Execute(0);
		}

		Server::m_ThreadCount.fetch_sub(1);
	});
}
