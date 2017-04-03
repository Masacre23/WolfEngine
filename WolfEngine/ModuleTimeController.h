#ifndef MODULETIMECONTROLLER_H
#define MODULETIMECONTROLLER_H

#include "Module.h"
#include <vector>

class GameObject;
class Timer;

enum TimeStates
{
	PLAY,
	PAUSE,
	STOP,
	TICK
};

class ModuleTimeController : public Module
{
public:
	ModuleTimeController();
	~ModuleTimeController();

	bool Init();
	update_status Update(float dt);
	void Play();
	void Pause();
	void Tick();
	void Stop();
	void UpdateDeltaTime();
	float GetTime();
	bool CleanUp();

public:
	std::vector<GameObject*> gameobjects;
	Timer* time = nullptr;
	TimeStates state;

	int frame_count; //app graphics frames since game start
	int game_time; //second since game start(Game Clock)
	float time_scale; //scale at which time is passing(Game Clock)
	float delta_time; //last frame time expressed in seconds(Game Clock)
	int real_time_since_startup; //seconds since game start(Real Time Clock)
	int real_time_delta_time; //last frame time expressed in seconds(Real Time Clock)
};

#endif