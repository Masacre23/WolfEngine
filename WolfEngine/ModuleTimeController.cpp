#include "ModuleTimeController.h"
#include "GameObject.h"
#include "ComponentTransform.h"

ModuleTimeController::ModuleTimeController() : Module("ModuleTimeController", true)
{
}


ModuleTimeController::~ModuleTimeController()
{
}

bool ModuleTimeController::Init()
{
	return true;
}

void ModuleTimeController::Play() const
{
	for (int i = 0; i < gameobjects.size(); ++i)
	{
		gameobjects[i]->transform->backup_local_transform = gameobjects[i]->transform->local_transform;
	}
}

void ModuleTimeController::Stop() const
{
	for (int i = 0; i < gameobjects.size(); ++i)
	{
		gameobjects[i]->transform->local_transform = gameobjects[i]->transform->backup_local_transform;
	}
}

bool ModuleTimeController::CleanUp()
{
	gameobjects.clear();
	return false;
}
