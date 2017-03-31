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
	vel = 1;
	Play();
	return true;
}

update_status ModuleTimeController::Update(float dt)
{
	if (state == PAUSE)
		this->dt = 0;
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
	dt = (float)time->GetTimeInMs() / 1000.0f;
	dt *= vel;
	time->Start();
}

bool ModuleTimeController::CleanUp()
{
	gameobjects.clear();
	return false;
}
