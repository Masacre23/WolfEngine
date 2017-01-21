#include "ModuleSceneIni.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

#include "SDL/include/SDL.h"

ModuleSceneIni::ModuleSceneIni(bool start_enabled) : Module("ModuleSceneIni",start_enabled)
{
	cat = new SDL_Rect({ 0, 0, 200, 200});
}

ModuleSceneIni::~ModuleSceneIni()
{}

// Load assets
bool ModuleSceneIni::Start()
{
	bool res = true;
	LOG("Loading initial scene");

	graphics = App->textures->Load("cat.png");

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
update_status ModuleSceneIni::Update()
{
	// Draw everything
	//App->renderer->BlitScreenCentered(graphics, cat);

	// Draw a screen-size black rectangle with alpha
	SDL_SetRenderDrawColor(App->renderer->renderer, 255.0f, 0, 0, 255.0f);
	SDL_RenderFillRect(App->renderer->renderer, NULL);

	return UPDATE_CONTINUE;
}