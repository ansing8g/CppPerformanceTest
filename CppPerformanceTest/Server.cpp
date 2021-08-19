#include "stdafx.h"
#include "Server.h"
#include "IFramework.h"
#include "NetworkThread.h"
#include "Task.h"

//--> static
Server* Server::Instance()
{
	if (nullptr == Server::m_pInstance)
	{
		Server::m_pInstance = new Server();
	}

	return Server::m_pInstance;
}

void Server::Destory()
{
	if (nullptr == Server::m_pInstance)
	{
		return;
	}

	delete Server::m_pInstance;
}

Server* Server::m_pInstance = nullptr;
Task Server::m_parrTaskPooling[TaskPoolingCount];
atomic_int Server::m_ThreadCount = 0;

//--> public
void Server::Start(IFrameWork* pFrameWork, int&& NetworkCount, int&& TaskCountPerSecond)
{
	m_pFrameWork = pFrameWork;
	m_NetworkCount = NetworkCount;
	m_TaskCountPerSecond = TaskCountPerSecond;

	for (int i = 0; i < TaskPoolingCount; ++i)
	{
		Server::m_parrTaskPooling[i].CreateJob();
	}

	int ThreadTaskCountPerSecond = 0 < TaskCountPerSecond ? TaskCountPerSecond / NetworkCount : 0;
	m_parrNetworkThread = new NetworkThread[NetworkCount];
	for (int i = 0; i < NetworkCount; ++i)
	{
		m_parrNetworkThread[i].Start(i + 1, ThreadTaskCountPerSecond);
	}
}

void Server::Running()
{
	chrono::system_clock::time_point now = chrono::system_clock::now();
	chrono::system_clock::time_point last = now;

	while (true)
	{
		now = chrono::system_clock::now();

		if (chrono::duration_cast<chrono::milliseconds>(now - last).count() >= 1000)
		{
			last = now;

			std::cout << "Execute ThreadCount=" << Server::m_ThreadCount.load() << std::endl;
		}

		this_thread::sleep_for(chrono::milliseconds(1));
	}
}

void Server::Receive(Task& _Task)
{
	if (nullptr == m_pFrameWork)
	{
		return;
	}

	m_pFrameWork->Receive(_Task);
}

void Server::Execute_Logic(int value)
{
	this_thread::sleep_for(chrono::milliseconds(value));
}

void Server::Execute_DB(int value)
{
	this_thread::sleep_for(chrono::milliseconds(value));
}

//--> private
Server::Server()
	: m_pFrameWork(nullptr)
	, m_NetworkCount(0)
	, m_TaskCountPerSecond(0)
	, m_parrNetworkThread(nullptr)
{

}

Server::~Server()
{
	if (nullptr != m_pFrameWork)
	{
		delete m_pFrameWork;
		m_pFrameWork = nullptr;
	}

	if (nullptr != m_parrNetworkThread)
	{
		delete m_parrNetworkThread;
		m_parrNetworkThread = nullptr;
	}
}
