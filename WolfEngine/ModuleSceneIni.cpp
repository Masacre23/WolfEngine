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
#include "ComponentRigidBody.h"
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

	std::list<std::string> pilot_animations;
	pilot_animations.push_back("ArmyPilot_Idle");
	pilot_animations.push_back("ArmyPilot_Run_Forwards");
	pilot_animations.push_back("ArmyPilot_Walk");

	/*App->level->ImportScene("Resources/Models/Batman/", "Batman.obj");
	App->level->ImportScene("Resources/Models/", "magnetto2.fbx");*/
	App->level->AddCamera();

	GameObject* city = App->level->ImportScene("Resources/Models/street/", "Street.obj");
	city->LoadRigidBody(Collider::Type::MESH, ComponentRigidBody::MotionType::STATIC);

	App->animations->Load("ArmyPilot_Idle", "Resources/Models/ArmyPilot/Animations/ArmyPilot_Idle.fbx");
	App->animations->Load("ArmyPilot_Run_Forwards", "Resources/Models/ArmyPilot/Animations/ArmyPilot_Run_Forwards.fbx");
	App->animations->Load("ArmyPilot_Walk", "Resources/Models/ArmyPilot/Animations/ArmyPilot_Walk.fbx");

	GameObject* pilot = App->level->ImportScene("Resources/Models/ArmyPilot/", "ArmyPilot.dae", true);
	if (pilot != nullptr)
	{
		pilot->SetLocalTransform(float3(0.0f, 2.0f, 0.0f));
		pilot->LoadAnimation(pilot_animations);
		pilot->LoadRigidBody(Collider::Type::BOX);
	}

	GameObject* cube = App->level->CreateGameObject("Resources/Lenna.png", PrimitiveCube(float3::one, float3(2.0f, 2.0f, 0.0f)), "LennaCube");
	cube->LoadRigidBody(Collider::Type::BOX);
	cube->CreateComponent(Component::Type::AUDIO_LISTENER);

	GameObject* sphere = App->level->CreateGameObject("Resources/Lenna.png", PrimitiveSphere(1.0f, float3(-2.0f, 2.0f, 0.0f)), "LennaSphere");
	sphere->LoadRigidBody(Collider::Type::SPHERE);
	
	//GameObject* plane = App->level->CreateGameObject("Resources/chess.jpg", PrimitivePlane(6.0f), "Floor");
	//plane->LoadRigidBody(Collider::Type::BOX, ComponentRigidBody::MotionType::STATIC);

	//GameObject* camera = App->level->CreateGameObject("TestCamera");
	//if (camera != nullptr)
	//{
	//	ComponentCamera* frustum_cam = (ComponentCamera*) camera->CreateComponent(Component::Type::CAMERA);
	//	//App->camera->frustum_camera = frustum_cam;
	//}

	/*AABB bbox = AABB();
	bbox.SetFromCenterAndSize(float3(0.0f, 0.0f, 0.0f), float3(10.0f, 10.0f, 10.0f));
	quad_tree = new MyQuadTree(bbox);*/

	//grass = new GameObject(App->level->GetRoot(), App->level->GetRoot(), "grass");
	//grass->CreateComponent(Component::Type::BILLBOARD);

	//rain = new GameObject(App->level->GetRoot(), App->level->GetRoot(), "rain");
	//rain->CreateComponent(Component::Type::PARTICLE);

	image = new GameObject(App->level->GetRoot(), App->level->GetRoot(), "Image");
	image->CreateComponent(Component::Type::RECT_TRANSFORM);
	image->CreateComponent(Component::Type::IMAGE);

	text = new GameObject(App->level->GetRoot(), App->level->GetRoot(), "Text");
	text->CreateComponent(Component::Type::RECT_TRANSFORM);
	text->CreateComponent(Component::Type::TEXT);

	App->program_shaders->Load("Prueba", "Resources/Shaders/vertex_shader.txt", "Resources/Shaders/fragment_shader.txt");

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