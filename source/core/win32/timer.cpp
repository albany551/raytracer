
#include <core/timer.hpp>

using namespace core;

Timer::Timer()
{
	LARGE_INTEGER freq, time0;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&time0);
	this->freq = static_cast<int64>(freq.QuadPart);
	this->time0 = static_cast<int64>(time0.QuadPart);
}


float Timer::GetTime() const
{
	LARGE_INTEGER time1;
	QueryPerformanceCounter(&time1);
	int64 delta = static_cast<int64>(time1.QuadPart) - time0;
	return (float)((double)(delta) / (double)(freq));
}


void Timer::Reset()
{
	LARGE_INTEGER time0;
	QueryPerformanceCounter(&time0);
	this->time0 = static_cast<int64>(time0.QuadPart);
}
