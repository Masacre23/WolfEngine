#ifndef TIMEERUS_H
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

	Uint32 GetTimeInUs() 
	{ 
		if (start)
			actual = SDL_GetPerformanceCounter();
		return (actual - start); 
	}


private:
	static const Uint64 frecuency = 0;
	Uint32 start_time;
	Uint32 actual;
	bool start;
};


#endif // !TIMERUS_H
