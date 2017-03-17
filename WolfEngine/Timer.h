#ifndef TIMER_H
#define TIMER_H

#include "SDL/include/SDL_timer.h"

class Timer
{
public:
	Timer() {};
	~Timer() {};

	void Start()
	{
		start_time = SDL_GetTicks();
		last_time = start_time;
		running = true;
	}

	void Stop()
	{
		if (running)
		{
			final_time = SDL_GetTicks();
			running = false;
		}
	}

	bool IsRunning() { return running; }

	Uint32 GetTimeInMs() const 
	{ 
		Uint32 ret;

		if (running)
			ret = SDL_GetTicks() - start_time;
		else
			ret = final_time - start_time;

		return ret; 
	}

	Uint32 DeltaTime() 
	{
		Uint32 ret;

		if (running)
		{
			Uint32 aux_time = SDL_GetTicks();
			ret =  aux_time - last_time;
			last_time = aux_time;
		}
		else
			ret = 0;

		return ret;
	}

	void static DelayInMs(Uint32 delay) 
	{
		SDL_Delay(delay);
	}

private:
	Uint32 start_time = 0;
	Uint32 final_time = 0;
	Uint32 last_time = 0;
	bool running = false;
};

#endif // !TIMER_H