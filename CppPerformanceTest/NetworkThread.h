#pragma once

using namespace std;

class Task;

class NetworkThread
{
private:
	void Worker();

private:
	static random_device RD;
	static mt19937 RandomGen;
	static uniform_int_distribution<int> RandomDis;

public:
	NetworkThread();
	~NetworkThread();

	void Start(int&& _Index, const int& _TaskCountPerSecond);
	void Stop();

private:
	thread m_Thread;
	bool m_Run;
	int m_TaskCountPerSecond;
	int m_Index;
	mutex m_Mutex;
	condition_variable m_ConditionVariable;
};
