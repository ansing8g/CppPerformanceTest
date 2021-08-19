#include "stdafx.h"
#include "Server.h"

#include "ConditionVariable.h"
#include "Leader_Followers.h"
#include "LockControl.h"
#include "UsingThreadPool.h"

int main()
{
	std::cout << "Server Start" << std::endl;

	unsigned int CoreCount = std::thread::hardware_concurrency();

	//IFrameWork* pFrameWork = nullptr;
	//IFrameWork* pFrameWork = reinterpret_cast<IFrameWork*>(new ConditionVariable(700));
	//IFrameWork* pFrameWork = reinterpret_cast<IFrameWork*>(new Leader_Followers(700));
	//IFrameWork* pFrameWork = reinterpret_cast<IFrameWork*>(new LockControl(700));
	IFrameWork* pFrameWork = reinterpret_cast<IFrameWork*>(new UsingThreadPool(700));

	Server::Instance()->Start(pFrameWork, 1, 1000);

	std::cout << "Server Running" << std::endl;
	Server::Instance()->Running();

	return 0;
}
