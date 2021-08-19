#pragma once

using namespace std;

class Task
{
private:
	class Job
	{
	public:
		Job(e_JobType& _JobType, int& _JobValue);
		~Job();

		void Execute();

	public:
		e_JobType m_JobType;
		int m_JobValue;

	private:
		static random_device RD;
		static mt19937 RandomGen;
		static uniform_int_distribution<int> RandomDis;
	};

private:
	static random_device RD;
	static mt19937 RandomGen;
	static uniform_int_distribution<int> RandomDis;

public:
	Task();
	~Task();

	void CreateJob();
	void AddJob(e_JobType&& jobType, int& jobValue);
	void Execute(int index);
	e_JobType NextJobType();
	void SetStartTime();

private:
	void Complete(int index);

public:
	list<Job> m_listJob;

private:
	chrono::system_clock::time_point m_start;
	chrono::system_clock::time_point m_end;
	string m_history;
	long long m_SettingDelay;
};

