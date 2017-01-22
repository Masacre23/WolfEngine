#include "ModuleSceneIni.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"


ModuleSceneIni::ModuleSceneIni(bool start_enabled) : Module("ModuleSceneIni",start_enabled)
{
	cat = new SDL_Rect({ 182, 100, 419, 380});
}

ModuleSceneIni::~ModuleSceneIni()
{}

// Load assets
bool ModuleSceneIni::Start()
{
	bool res = true;
	LOG("Loading initial scene");

	//graphics = App->textures->Load("Resources/cat.png");

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
	//App->renderer->BlitScreenCentered(graphics, cat);

	return UPDATE_CONTINUE;
}