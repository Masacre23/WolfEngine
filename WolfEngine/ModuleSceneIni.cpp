#include "ModuleSceneIni.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleLevel.h"
#include "Model.h"
#include "ModuleEditor.h"
#include "GameObject.h"

ModuleSceneIni::ModuleSceneIni(bool start_enabled) : Module("ModuleSceneIni",start_enabled)
{}

ModuleSceneIni::~ModuleSceneIni()
{}

// Load assets
bool ModuleSceneIni::Start()
{
	bool res = true;
	LOG("Loading initial scene");

	batman = new Model("Batman");
	batman->Load("Resources/Models/Batman/","Batman.obj");

	magnetto = new Model("magnetto2");
	magnetto->Load("Resources/Models/", "magnetto2.fbx");

	App->level->ImportScene("Resources/Models/street/", "Street.obj");
	App->editor->game_objects_labels.push_back(App->level->GetRoot()->name);
	/*for(int i = 0; i < App->level->GetRootNode()->childs.size(); ++i)
		App->editor->game_objects_labels.push_back(App->level->GetRootNode()->childs[i]->name);*/

	return res;
}

// Unload assets
bool ModuleSceneIni::CleanUp()
{
	LOG("Unloading initial scene");

	RELEASE(batman);
	RELEASE(magnetto);

	return true;
}

// Draw the elements of the scene
update_status ModuleSceneIni::Update(float dt)
{
	App->renderer->DrawCube(App->textures->texture_debug, { 2.0f, 0.0f, 0.0f });
	App->renderer->DrawCube(App->textures->texture_checkers, { -2.0f, 0.0f, 0.0f });

	batman->Draw();
	magnetto->Draw();

	App->level->Draw();

	return UPDATE_CONTINUE;
}