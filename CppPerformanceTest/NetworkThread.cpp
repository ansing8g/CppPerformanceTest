#include "stdafx.h"
#include "NetworkThread.h"
#include "Server.h"
#include "Task.h"

//--> private
void NetworkThread::Worker()
{
	chrono::system_clock::time_point start, end;
	while (this->m_Run)
	{
		start = chrono::system_clock::now();

		for (int i = 0; i < this->m_TaskCountPerSecond; ++i)
		{
			Task& _Task = Server::m_parrTaskPooling[NetworkThread::RandomDis(NetworkThread::RandomGen)];
			_Task.SetStartTime();
			Server::Instance()->Receive(_Task);
		}

		end = chrono::system_clock::now();

		unique_lock<mutex> Lock(this->m_Mutex);
		this->m_ConditionVariable.wait_for(Lock, chrono::duration_cast<chrono::milliseconds>(start + chrono::milliseconds(1000) - end));
		Lock.unlock();

		//if (1 == this->m_Index)
		//{
		//	cout << "Recv Execute Ticks=" << chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start).count() << " Run ThreadCount=" << Server::m_ThreadCount << endl;
		//}
	}
}

random_device NetworkThread::RD;
mt19937 NetworkThread::RandomGen(NetworkThread::RD());
uniform_int_distribution<int> NetworkThread::RandomDis(0, TaskPoolingCount - 1);

//--> public
NetworkThread::NetworkThread()
	: m_Thread()
	, m_Run(true)
	, m_TaskCountPerSecond(0)
	, m_Index(0)
	, m_Mutex()
	, m_ConditionVariable()
{

}

NetworkThread::~NetworkThread()
{
	m_Run = false;
	m_Thread.join();
}

void NetworkThread::Start(int&& _Index, const int& _TaskCountPerSecond)
{
	m_Run = true;
	m_Index = _Index;
	m_TaskCountPerSecond = _TaskCountPerSecond;
	m_Thread = thread(&NetworkThread::Worker, this);
}

void NetworkThread::Stop()
{
	m_Run = false;
}
