#include "ModuleSceneIni.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleLevel.h"
#include "ModuleAnimations.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include "PanelInterface.h"

ModuleSceneIni::ModuleSceneIni(bool start_enabled) : Module("ModuleSceneIni",start_enabled)
{}

ModuleSceneIni::~ModuleSceneIni()
{}

// Load assets
bool ModuleSceneIni::Start()
{
	bool res = true;
	LOG("Loading initial scene");

	//App->level->ImportScene("Resources/Models/Batman/", "Batman.obj");
	//App->level->ImportScene("Resources/Models/", "magnetto2.fbx");
	//App->level->ImportScene("Resources/Models/street/", "Street.obj");
	pilot = App->level->ImportScene("Resources/Models/ArmyPilot/", "ArmyPilot.dae");

	if (pilot != nullptr)
	{
		pilot->LoadAnim("ArmyPilot_Idle", "Resources/Models/ArmyPilot/Animations/ArmyPilot_Idle.fbx");
		App->animations->Load("ArmyPilot_Run_Forwards", "Resources/Models/ArmyPilot/Animations/ArmyPilot_Run_Forwards.fbx");
		App->animations->Load("ArmyPilot_Walk", "Resources/Models/ArmyPilot/Animations/ArmyPilot_Walk.fbx");
	}	

	return res;
}

// Unload assets
bool ModuleSceneIni::CleanUp()
{
	LOG("Unloading initial scene");

	return true;
}

// Draw the elements of the scene
update_status ModuleSceneIni::Update(float dt)
{
	static int animation = 0;

	App->renderer->DrawCube(App->textures->texture_debug, { 2.0f, 0.0f, 0.0f });
	App->renderer->DrawCube(App->textures->texture_checkers, { -2.0f, 0.0f, 0.0f });

	App->level->Draw();

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		animation = (animation + 1) % 3;
		if (animation == 0) 
			pilot->ChangeAnim(anim_idle, 200); 
		else if (animation == 1)
			pilot->ChangeAnim(anim_walk, 200);
		else
			pilot->ChangeAnim(anim_run, 200);
	}


	return UPDATE_CONTINUE;
}