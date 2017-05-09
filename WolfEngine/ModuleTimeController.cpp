#include "ModuleTimeController.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Timer.h"
#include "TimerUs.h"
#include "Application.h"
#include "ModuleLevel.h"
#include "ModuleWindow.h"
#include "JsonHandler.h"

ModuleTimeController::ModuleTimeController() : Module(MODULE_TIME, true)
{
}


ModuleTimeController::~ModuleTimeController()
{
}

bool ModuleTimeController::Init()
{
	game_timer = new Timer();
	update_timer = new Timer();
	app_timer = new Timer();

	if (App->parser->LoadObject(TIME_SECTION))
	{
		fps_cap = App->parser->GetInt("FpsCap");
		cap_ms = 1000 / fps_cap;
		App->parser->UnloadObject();
	}

	app_timer->Start();

	return true;
}

bool ModuleTimeController::CleanUp()
{
	RELEASE(game_timer);
	RELEASE(update_timer);
	RELEASE(app_timer);

	return false;
}

update_status ModuleTimeController::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleTimeController::PostUpdate(float dt)
{
	BROFILER_CATEGORY("ModuleTimeController-PostUpdate", Profiler::Color::Green);

	/*if (state == PAUSE || state == STOP)
	this->delta_time = 0;*/
	if (game_state == TICK)
		game_state = PAUSE;

	return UPDATE_CONTINUE;
}

void ModuleTimeController::Play()
{
	App->level->SaveGameObjects();
	
	game_timer->Start();

	game_state = PLAY;
}

void ModuleTimeController::Pause()
{
	game_timer->Stop();

	game_state = PAUSE;
}

void ModuleTimeController::Tick()
{
	if (game_state == PAUSE)
		game_state = TICK;
}

void ModuleTimeController::Stop()
{
	App->level->RestoreGameObjects();

	frame_count = 0;
	game_timer->Stop();

	game_state = STOP;
}

float ModuleTimeController::UpdateDeltaTime()
{
	delta_time = (float)update_timer->GetTimeInMs() / 1000.0f;
	real_time_delta_time = delta_time;
	delta_time *= time_scale;

	if (game_state == STOP || game_state == PAUSE)
		delta_time = 0.0f;
	
	update_timer->Start();

	return delta_time;
}

void ModuleTimeController::EndUpdate()
{
	// Amount of time of the last frame
	last_frame_ms = update_timer->GetTimeInMs();
	// Amount of frames since app startup
	real_frame_count++;
	counter_frames++;
	// Amount of frames since game startup
	if (game_state == PLAY)
		frame_count++;

	// Amount of time since game start
	game_time = ((float)game_timer->GetTimeInMs()) / 1000.0f;
	// Amount of time since app start
	real_time_since_startup = ((float)app_timer->GetTimeInMs()) / 1000.0f;

	// Amount of frames during the last second (the actual FPS)
	if (real_time_since_startup - prev_time > 1.0f)
	{
		frames_last_sec = counter_frames;
		counter_frames = 0;
		prev_time = real_time_since_startup;
	}

	App->window->SetFPStoWindow(real_frame_count, real_time_since_startup, last_frame_ms, frames_last_sec, real_time_delta_time);

	//Calculate the time for the next frame.
	TimerUs delay_timer;
	if (last_frame_ms < cap_ms)
	{
		int time_to_nframe = cap_ms - last_frame_ms;
		delay_timer.Start();
		Timer::DelayInMs(time_to_nframe);
		float real_delay_time = (float)delay_timer.GetTimeInMs();
		//LOG("We wait for %i milliseconds and got back in %f", time_to_nframe, real_delay_time);
	}
}

void ModuleTimeController::SetFpsCap(int fps)
{
	fps_cap = fps;
	cap_ms = 1000 / fps_cap;
}
