#ifndef TIMERUS_H
#define TIMERUS_H

#include "SDL/include/SDL_timer.h"


class TimerUs
{
public:
	TimerUs(){};
	~TimerUs() {};

	void Start()
	{
		start_time = SDL_GetPerformanceCounter();
		start = true;
	}

	void Stop()
	{
		start = false;
	}

	Uint64 GetTimeInUs() 
	{ 
		if (start)
			actual = SDL_GetPerformanceCounter();
		return ((actual - start_time)*1000000/frecuency); 
	}

public:
	static Uint64 frecuency;

private:
	Uint64 start_time;
	Uint64 actual;
	bool start;
};

#endif // !TIMERUS_H
