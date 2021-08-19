#include "stdafx.h"
#include "Logger.h"

mutex Logger::m_lock;

//--> public
void Logger::Write(const string& history, long long& setDelay, long long& delay)
{
	lock_guard<mutex> guard(Logger::m_lock);

	ofstream ofs(LogFileName, ios::app);
	if (false == ofs.is_open())
	{
		return;
	}

	ofs << Logger::CurrentTimeToString() << ", " << history.c_str() << ", " << setDelay << ", " << delay << endl;
	ofs.close();
}

void Logger::Write(string&& message, int& index)
{
	lock_guard<mutex> guard(Logger::m_lock);

	ofstream ofs(LogFileName, ios::app);
	if (false == ofs.is_open())
	{
		return;
	}

	ofs << Logger::CurrentTimeToString() << ", " << message.c_str() << ", " << index << endl;
	ofs.close();
}

//--> private
string Logger::TimepointToString(const chrono::system_clock::time_point& p_tpTime)
{
	time_t timet = chrono::system_clock::to_time_t(p_tpTime);

	struct tm tmTime;
	errno_t error = localtime_s(&tmTime, &timet);

	ostringstream oss;
	oss << put_time(&tmTime, "%Y_%m_%d %H:%M:%S");

	return oss.str();
}

string Logger::CurrentTimeToString()
{
	return Logger::TimepointToString(chrono::system_clock::now());
}
