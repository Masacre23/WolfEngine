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
	float dt;
	float vel;
};

#endif