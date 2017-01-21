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
		running = true;
	}

	void Stop()
	{
		if (running)
		{
			final_time = SDL_GetPerformanceCounter();
			running = false;
		}
	}

	Uint64 GetTimeInUs() 
	{
		Uint64 ret;

		if (running)
			ret = SDL_GetPerformanceCounter() - start_time;
		else
			ret = final_time - start_time;

		return (ret*1000000/frecuency); 
	}

	Uint64 GetTimeInMs()
	{
		Uint64 ret;

		if (running)
			ret = SDL_GetPerformanceCounter() - start_time;
		else
			ret = final_time - start_time;

		return (ret * 1000 / frecuency);
	}

public:
	static Uint64 frecuency;

private:
	Uint64 start_time;
	Uint64 final_time;
	bool running = false;
};

#endif // !TIMERUS_H
