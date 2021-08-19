#pragma once

using namespace std;

class Logger
{
public:
	static void Write(const string& history, long long& setDealy, long long& delay);
	static void Write(string&& message, int& index);
	
private:
	static string TimepointToString(const chrono::system_clock::time_point& p_tpTime);
	static string CurrentTimeToString();

private:
	static mutex m_lock;
};
