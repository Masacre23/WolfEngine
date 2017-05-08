#include "ModuleSceneIni.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleLevel.h"
#include "ModuleAnimations.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleProgramShaders.h"
#include "GameObject.h"
#include "PanelInterface.h"
#include "MyQuadTree.h"
#include "Billboard.h"
#include "ComponentParticleSystem.h"
#include "Primitive.h"

ModuleSceneIni::ModuleSceneIni(bool start_enabled) : Module("ModuleSceneIni",start_enabled)
{}

ModuleSceneIni::~ModuleSceneIni()
{}

// Load assets
bool ModuleSceneIni::Start()
{
	bool res = true;
	APPLOG("Loading initial scene");

	/*App->level->ImportScene("Resources/Models/Batman/", "Batman.obj");
	App->level->ImportScene("Resources/Models/", "magnetto2.fbx");*/
	App->level->AddCamera();
	App->level->ImportScene("Resources/Models/street/", "Street.obj");

	/*pilot = App->level->ImportScene("Resources/Models/ArmyPilot/", "ArmyPilot.dae", true);
	if (pilot != nullptr)
	{
		pilot->LoadAnim("ArmyPilot_Idle", "Resources/Models/ArmyPilot/Animations/ArmyPilot_Idle.fbx");
		App->animations->Load("ArmyPilot_Run_Forwards", "Resources/Models/ArmyPilot/Animations/ArmyPilot_Run_Forwards.fbx");
		App->animations->Load("ArmyPilot_Walk", "Resources/Models/ArmyPilot/Animations/ArmyPilot_Walk.fbx");
	}	*/

	GameObject* cube = App->level->CreateGameObject("Resources/Lenna.png", &Primitives::Cube, "LennaCube");
	cube->SetLocalTransform({ 2.0f, 0.0f, 0.0f });

	//GameObject* camera = App->level->CreateGameObject("TestCamera");
	//if (camera != nullptr)
	//{
	//	ComponentCamera* frustum_cam = (ComponentCamera*) camera->CreateComponent(Component::Type::CAMERA);
	//	//App->camera->frustum_camera = frustum_cam;
	//}

	/*AABB bbox = AABB();
	bbox.SetFromCenterAndSize(float3(0.0f, 0.0f, 0.0f), float3(10.0f, 10.0f, 10.0f));
	quad_tree = new MyQuadTree(bbox);*/

	grass = new GameObject(App->level->GetRoot(), App->level->GetRoot(), "grass");
	grass->CreateComponent(Component::Type::BILLBOARD);

	rain = new GameObject(App->level->GetRoot(), App->level->GetRoot(), "rain");
	rain->CreateComponent(Component::Type::PARTICLE);

	App->program_shaders->Load("Prueba", "varying vec2 myTexCoord;\n\nvoid main()\n{\ngl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex;\nmyTexCoord = vec2(gl_MultiTexCoord0);\n}\n", "varying vec2 myTexCoord;\nuniform sampler2D diffuse;\n\nvoid main()\n{\ngl_FragColor = texture2D(diffuse, myTexCoord);\n}");

	return res;
}

// Unload assets
bool ModuleSceneIni::CleanUp()
{
	APPLOG("Unloading initial scene");
	//RELEASE(quad_tree);

	return true;
}

// Draw the elements of the scene
update_status ModuleSceneIni::Update(float dt)
{
	static int animation = 0;

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

	/*if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		float x = -5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10)));
		float z = -5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10)));
		GameObject* g = App->level->CreateGameObject("Empty");
		empty_game_objects.push_back(g);
		g->SetLocalTransform(float3(x, 0.0f, z), float3(1.0f, 1.0f, 1.0f), Quat(0.0f, 0.0f, 0.0f, 1.0f));
		AABB box = AABB();
		box.SetFromCenterAndSize(float3(0.0f, 0.0f, 0.0f), float3(0.5f, 0.5f, 0.5f));
		g->RecursiveUpdateTransforms();
		g->SetAABB(box);
		quad_tree->Insert(g);
	}

	for(int i = 0; i < empty_game_objects.size(); i++)
	{
		empty_game_objects[i]->Draw();
	}
	
	quad_tree->Draw();*/

	return UPDATE_CONTINUE;
}