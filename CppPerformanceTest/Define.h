#pragma once

#define LogFileName "Log.txt"

enum class e_JobType
{
	None,
	Logic,
	DB,
};

static constexpr int Percent = 10000;
static constexpr int TaskPoolingCount = 1000000;
static constexpr int Job_Count_Min = 1;
static constexpr int Job_Count_Max = 5;
static constexpr int Logic_DB_Percent = 40 * Percent / 100;
static constexpr int Logic_Delay_Min_MilliSecond = 1;
static constexpr int Logic_Delay_Max_MilliSecond = 50;
static constexpr int DB_Delay_Min_MilliSecond = 10;
static constexpr int DB_Delay_Max_MilliSecond = 1000;
