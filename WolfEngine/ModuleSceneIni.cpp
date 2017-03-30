#include "ModuleSceneIni.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleLevel.h"
#include "ModuleAnimations.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "GameObject.h"
#include "PanelInterface.h"
#include "MyQuadTree.h"
#include "Billboard.h"
#include "Grass.h"

ModuleSceneIni::ModuleSceneIni(bool start_enabled) : Module("ModuleSceneIni",start_enabled)
{}

ModuleSceneIni::~ModuleSceneIni()
{}

// Load assets
bool ModuleSceneIni::Start()
{
	bool res = true;
	APPLOG("Loading initial scene");

	//App->level->ImportScene("Resources/Models/Batman/", "Batman.obj");
	//App->level->ImportScene("Resources/Models/", "magnetto2.fbx");
	/*App->level->ImportScene("Resources/Models/street/", "Street.obj");
	pilot = App->level->ImportScene("Resources/Models/ArmyPilot/", "ArmyPilot.dae");

	if (pilot != nullptr)
	{
		pilot->LoadAnim("ArmyPilot_Idle", "Resources/Models/ArmyPilot/Animations/ArmyPilot_Idle.fbx");
		App->animations->Load("ArmyPilot_Run_Forwards", "Resources/Models/ArmyPilot/Animations/ArmyPilot_Run_Forwards.fbx");
		App->animations->Load("ArmyPilot_Walk", "Resources/Models/ArmyPilot/Animations/ArmyPilot_Walk.fbx");
	}	*/

	//GameObject* camera = App->level->CreateGameObject("TestCamera");
	//if (camera != nullptr)
	//{
	//	ComponentCamera* frustum_cam = (ComponentCamera*) camera->CreateComponent(Component::Type::CAMERA);
	//	//App->camera->frustum_camera = frustum_cam;
	//}

	AABB bbox = AABB();
	bbox.SetFromCenterAndSize(float3(0.0f, 0.0f, 0.0f), float3(10.0f, 10.0f, 10.0f));
	quad_tree = new MyQuadTree(bbox);

	//grass = new Billboard(aiString("Resources/billboardgrass.png"), {0.0f, 0.0f, 0.0f}, 0.5, 0.5);

	//grass = new Grass(10, 10);

	//grass->Start();

	return res;
}

// Unload assets
bool ModuleSceneIni::CleanUp()
{
	APPLOG("Unloading initial scene");
	RELEASE(quad_tree);

	return true;
}

// Draw the elements of the scene
update_status ModuleSceneIni::Update(float dt)
{
	static int animation = 0;

	App->renderer->DrawCube(App->textures->texture_debug, { 2.0f, 0.0f, 0.0f });
	App->renderer->DrawCube(App->textures->texture_checkers, { -2.0f, 0.0f, 0.0f });

	App->level->Draw();

	/*if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		animation = (animation + 1) % 3;
		if (animation == 0) 
			pilot->ChangeAnim(anim_idle, 200); 
		else if (animation == 1)
			pilot->ChangeAnim(anim_walk, 200);
		else
			pilot->ChangeAnim(anim_run, 200);
	}*/

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		float x = -10 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - -10)));
		float z = -10 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - -10)));
		GameObject* g = App->level->CreateGameObject("Empty");
		empty_game_objects.push_back(g);
		g->SetLocalTransform(float3(x, 0.0f, z), float3(0.0f, 0.0f, 0.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f));
		AABB box = AABB();
		box.SetFromCenterAndSize(float3(0.0f, 0.0f, 0.0f), float3(0.1f, 0.1f, 0.1f));
		g->SetAABB(box);
		quad_tree->Insert(g);
	}

	quad_tree->Draw();

	//grass->ComputeQuad(App->camera->GetPosition());
	/*grass->Draw();*/

	return UPDATE_CONTINUE;
}