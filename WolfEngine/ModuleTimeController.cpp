#include "ModuleTimeController.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Timer.h"
#include "Application.h"
#include "ModuleLevel.h"

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

	state = PAUSE;

	return true;
}

bool ModuleTimeController::Start()
{
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
	App->level->SaveGameObjectsTransforms();
	
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
	App->level->RestoreGameObjectsTransforms();

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
	RELEASE(time);

	return false;
}
