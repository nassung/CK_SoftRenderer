
#include "WindowsPrecompiled.h"
#include "WindowsTimer.h"

double WindowsTimer::Init()
{
	LARGE_INTEGER frequency;
	if (!QueryPerformanceFrequency(&frequency)) 
	{
		return 0;
	}

	return double(frequency.QuadPart) / 1000.0;
}

__int64 WindowsTimer::CheckMilliSeconds()
{
	LARGE_INTEGER cycles;
	QueryPerformanceCounter(&cycles);
	return cycles.QuadPart;
}
