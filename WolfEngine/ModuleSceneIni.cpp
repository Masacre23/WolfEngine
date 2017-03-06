#include "ModuleSceneIni.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleLevel.h"
#include "Model.h"
#include "ModuleEditor.h"
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

	batman = new Model("Batman");
	batman->Load("Resources/Models/Batman/","Batman.obj");

	magnetto = new Model("magnetto2");
	magnetto->Load("Resources/Models/", "magnetto2.fbx");

	App->level->ImportScene("Resources/Models/street/", "Street.obj");
	//App->level->CreateGameObject("Resources/Models/Batman/", "Batman.obj");

	empty_gameobject = App->level->CreateGameObject(nullptr, "empty_gameobject");
	empty_gameobject->CreateComponent(Component::Type::TRANSFORM);

	child_gameobject = App->level->CreateGameObject(empty_gameobject, "child_1");
	child_gameobject->CreateComponent(Component::Type::TRANSFORM);

	child_gameobject2 = App->level->CreateGameObject(empty_gameobject, "child_2");
	child_gameobject2->CreateComponent(Component::Type::TRANSFORM);

	child_child_gameobject = App->level->CreateGameObject(child_gameobject, "child_child1");
	child_child_gameobject->CreateComponent(Component::Type::TRANSFORM);

	child_child_gameobject2 = App->level->CreateGameObject(child_gameobject, "child_child2");
	child_child_gameobject2->CreateComponent(Component::Type::TRANSFORM);

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
	//magnetto->Draw();

	App->level->Draw();

	return UPDATE_CONTINUE;
}