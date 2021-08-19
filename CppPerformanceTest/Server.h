#pragma once

using namespace std;

class IFrameWork;
class NetworkThread;
class Task;

class Server
{
//--> static
public:
	static Server* Instance();
	static void Destory();

private:
	static Server* m_pInstance;

public:
	static Task m_parrTaskPooling[TaskPoolingCount];
	static atomic_int m_ThreadCount;

public:
	inline const IFrameWork& Logic() { return *m_pFrameWork; }

	void Start(IFrameWork* pFrameWork, int&& NetworkCount, int&& TaskCountPerSecond);
	void Running();

	void Receive(Task& _Task);

	void Execute_Logic(int value);
	void Execute_DB(int value);

private:
	Server();
	~Server();

private:
	IFrameWork* m_pFrameWork;
	int m_NetworkCount;
	int m_TaskCountPerSecond;
	NetworkThread* m_parrNetworkThread;
};
