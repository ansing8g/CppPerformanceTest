#include "stdafx.h"
#include "Task.h"
#include "Logger.h"

//--> Job
random_device Task::Job::RD;
mt19937 Task::Job::RandomGen(Task::Job::RD());
uniform_int_distribution<int> Task::Job::RandomDis(0, Percent);

Task::Job::Job(e_JobType& _JobType, int& _JobValue)
	: m_JobType(_JobType)
	, m_JobValue(_JobValue)
{

}

Task::Job::~Job()
{
	m_JobType = e_JobType::None;
	m_JobValue = 0;
}

void Task::Job::Execute()
{
	if (e_JobType::Logic == m_JobType)
	{
		unsigned long temp = 0;
		chrono::system_clock::time_point joblimit = chrono::system_clock::now() + chrono::milliseconds(m_JobValue);
		while (chrono::system_clock::now() < joblimit)
		{
			temp += Task::Job::RandomDis(RandomGen);
			temp -= Task::Job::RandomDis(RandomGen);
		}
	}
	else if (e_JobType::DB == m_JobType)
	{
		this_thread::sleep_for(chrono::milliseconds(m_JobValue));
	}
}

//--> Task
//--> static
random_device Task::RD;
mt19937 Task::RandomGen(Task::RD());
uniform_int_distribution<int> Task::RandomDis(0, Percent);

Task::Task()
	: m_listJob()
	, m_start(chrono::time_point<chrono::system_clock>::max())
	, m_end(chrono::time_point<chrono::system_clock>::min())
	, m_history()
	, m_SettingDelay(0)
{

}

Task::~Task()
{
	m_listJob.clear();
	m_SettingDelay = 0;
}

void Task::CreateJob()
{
	m_listJob.clear();
	m_SettingDelay = 0;

	int rand_jobcount = (Task::RandomDis(Task::RandomGen) % (Job_Count_Max - Job_Count_Min + 1)) + Job_Count_Min;
	for (int i = 0; i < rand_jobcount; ++i)
	{
		int rand_logic = Task::RandomDis(Task::RandomGen);
		if (rand_logic <= Logic_DB_Percent)
		{
			int rand_db_delay = (Task::RandomDis(Task::RandomGen) % (DB_Delay_Max_MilliSecond - DB_Delay_Min_MilliSecond + 1)) + DB_Delay_Min_MilliSecond;
			AddJob(e_JobType::DB, rand_db_delay);
		}
		else
		{
			int rand_logic_delay = (Task::RandomDis(Task::RandomGen) % (Logic_Delay_Max_MilliSecond - Logic_Delay_Min_MilliSecond + 1)) + Logic_Delay_Min_MilliSecond;
			AddJob(e_JobType::Logic, rand_logic_delay);
		}
	}
}

void Task::AddJob(e_JobType&& jobType, int& jobValue)
{
	m_listJob.push_back(Job(jobType, jobValue));
	m_SettingDelay += jobValue;
}

void Task::Execute(int index)
{
	if (0 >= m_listJob.size())
	{
		return;
	}

	Job job = m_listJob.front();
	m_listJob.pop_front();

	job.Execute();

	switch (job.m_JobType)
	{
	case e_JobType::Logic:
	{
		m_history += "[Logic:" + to_string(job.m_JobValue) + "]";
	} break;
	case e_JobType::DB:
	{
		m_history += "[DB:" + to_string(job.m_JobValue) + "]";
	} break;
	}

	if (0 >= m_listJob.size())
	{
		m_end = max(m_end, chrono::system_clock::now());
		this->Complete(index);
	}
}

e_JobType Task::NextJobType()
{
	if (0 >= m_listJob.size())
	{
		return e_JobType::None;
	}

	return m_listJob.begin()->m_JobType;
}

void Task::SetStartTime()
{
	m_start = min(m_start, chrono::system_clock::now());
}

//--> private
void Task::Complete(int index)
{
	long long diff = chrono::duration_cast<chrono::milliseconds>(m_end - m_start).count();

	m_history = string("Index=") + to_string(index) + ", " + m_history;
	Logger::Write(m_history, m_SettingDelay, diff);
}
