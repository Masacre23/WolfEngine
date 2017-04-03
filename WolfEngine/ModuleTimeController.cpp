#include "ModuleTimeController.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Timer.h"
#include "Application.h"

ModuleTimeController::ModuleTimeController() : Module("ModuleTimeController", true)
{
}


ModuleTimeController::~ModuleTimeController()
{
}

bool ModuleTimeController::Init()
{
	time = new Timer();
	frame_count = 0;
	game_time = 0; 
	time_scale = 1.0f;
	delta_time = 0.0f;
	Play();
	return true;
}

update_status ModuleTimeController::Update(float dt)
{
	if (state == PAUSE)
		this->delta_time = 0;
	if (state == TICK)
		state = PAUSE;
	return UPDATE_CONTINUE;
}

void ModuleTimeController::Play()
{
	for (int i = 0; i < gameobjects.size(); ++i)
	{
		gameobjects[i]->transform->backup_local_transform = gameobjects[i]->transform->local_transform;
	}
	state = PLAY;
}

void ModuleTimeController::Pause()
{
	Stop();
	state = PAUSE;
}

void ModuleTimeController::Tick()
{
	if (state == PAUSE)
	{
		state = TICK;
	}
}

void ModuleTimeController::Stop()
{
	for (int i = 0; i < gameobjects.size(); ++i)
	{
		gameobjects[i]->transform->local_transform = gameobjects[i]->transform->backup_local_transform;
	}
	state = STOP;
}

void ModuleTimeController::UpdateDeltaTime()
{
	delta_time = (float)time->GetTimeInMs() / 1000.0f;
	delta_time *= time_scale;
	time->Start();
}

bool ModuleTimeController::CleanUp()
{
	gameobjects.clear();
	return false;
}
