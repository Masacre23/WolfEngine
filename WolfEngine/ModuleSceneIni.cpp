#include "ModuleSceneIni.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

ModuleSceneIni::ModuleSceneIni(bool start_enabled) : Module("ModuleSceneIni",start_enabled)
{}

ModuleSceneIni::~ModuleSceneIni()
{}

// Load assets
bool ModuleSceneIni::Start()
{
	bool res = true;
	LOG("Loading initial scene");

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
	App->renderer->DrawCube(App->textures->texture_debug);
	App->renderer->DrawCube(App->textures->texture_checkers, { -2.0f, 0.0f, 0.0f });

	return UPDATE_CONTINUE;
}