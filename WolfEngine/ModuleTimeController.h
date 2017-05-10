#ifndef MODULETIMECONTROLLER_H
#define MODULETIMECONTROLLER_H

#define MODULE_TIME "ModuleTimeController"
#define TIME_SECTION "Config.Modules.TimeController"

#include "Module.h"
#include <vector>

class Timer;

class ModuleTimeController : public Module
{
public:
	enum TimeStates
	{
		PLAY,
		PAUSE,
		STOP,
		TICK
	};

public:
	ModuleTimeController();
	~ModuleTimeController();

	bool Init();
	bool CleanUp();
	update_status PreUpdate(float dt);

	void Play();
	void Pause();
	void Tick();
	void Stop();

	float UpdateDeltaTime();
	void EndUpdate();

	bool IsGameRunning() const;

	int GetFpsCap() const { return fps_cap; }
	void SetFpsCap(int fps);

	int GetFramesLastSecond() const { return frames_last_sec; }
	int GetLastFrameMs() const { return last_frame_ms; }

	int GetFrameCount() const { return frame_count; }
	float GetGameTime() const { return game_time; }
	float GetDeltaTime() const { return delta_time; }
	float GetRealDeltaTime() const { return real_time_delta_time; }

	bool IsPlaying() const { return game_state == TimeStates::PLAY; }
	bool IsStopped() const { return game_state == TimeStates::STOP; }

public:
	float time_scale = 1.0f; // scale at which time is passing (Game Clock

private:
	Timer* game_timer = nullptr;
	Timer* update_timer = nullptr;
	Timer* app_timer = nullptr;

	TimeStates game_state = STOP;

	int frame_count = 0; // app graphics frames since game start (Game)
	float game_time = 0; // second since game start (Game Clock)
	float delta_time = 0.0f; // last frame time expressed in seconds (Game Clock)

	int real_frame_count = 0; // app graphics frames since app start (Real Time)
	float real_time_since_startup = 0; // seconds since game start (Real Time Clock)
	float real_time_delta_time = 0.0f; // last frame time expressed in seconds (Real Time Clock)
	int frames_last_sec = 0;
	int last_frame_ms = 0;

	int fps_cap = 60;
	int cap_ms = 1000;

	int counter_frames = 0;
	float prev_time = 0;
};

#endif