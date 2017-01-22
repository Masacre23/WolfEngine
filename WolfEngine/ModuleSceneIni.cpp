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

	graphics = App->textures->Load("Resources/airou.png");

	return res;
}

// Unload assets
bool ModuleSceneIni::CleanUp()
{
	LOG("Unloading initial scene");

	App->textures->Unload(graphics);

	return true;
}

// Draw the elements of the scene
update_status ModuleSceneIni::Update(float dt)
{
	// Draw everything
	App->renderer->BlitScreenCentered(graphics, NULL);

	return UPDATE_CONTINUE;
}