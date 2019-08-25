#include "Math.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleLevel.h"
#include "ModuleTimeController.h"
#include "Application.h"
#include "ComponentCamera.h"
#include "SDL/include/SDL.h"
#include "JsonHandler.h"

ModuleCamera::ModuleCamera() : Module(MODULE_CAMERA)
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	editor_camera = new ComponentCamera();
	SetupFrustum(editor_camera);

	rendering_camera = editor_camera;

	return true;
}

bool ModuleCamera::Start()
{
	float3 initial_pos = float3::zero;
	if (App->parser->LoadObject(CAMERA_SECTION))
	{
		NEARPLANE = App->parser->GetFloat("NearPlane");
		FARPLANE = App->parser->GetFloat("FarPlane");
		VERTICALFOV = App->parser->GetFloat("VerticalFOV");
		ASPECTRATIO = App->parser->GetFloat("AspectRatio");

		SPEED_ROTATION = App->parser->GetFloat("RotationSpeed");
		SPEED_TRANSLATION = App->parser->GetFloat("TranslationSpeed");
		SPEED_ZOOM = App->parser->GetFloat("ZoomSpeedFactor");
		App->parser->GetVector3("InitialPosition", &initial_pos);

		App->parser->UnloadObject();
	}

	editor_camera->SetPosition(initial_pos);
	editor_camera->LookAt(float3::zero);

	return true;
}

update_status ModuleCamera::Update(float dt)
{
	BROFILER_CATEGORY("ModuleCamera-Update", Profiler::Color::Red);

	if (rendering_camera == editor_camera)
	{
		float real_dt = App->time_controller->GetRealDeltaTime();

		float3 movement = float3::zero;
		float3 direction_forward = editor_camera->frustum->Front();
		float3 direction_right = editor_camera->frustum->WorldRight();
		float3 direction_up = float3::unitY;
		bool shift_pressed = (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT);

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += direction_forward;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement -= direction_forward;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) movement -= direction_right;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			movement += direction_right;
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) movement -= direction_up;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) movement += direction_up;

		movement += direction_forward * App->input->mouse_wheel.y * SPEED_ZOOM;
		float3 translation = movement * SPEED_TRANSLATION * (shift_pressed ? 2 : 1) * real_dt;

		// Camera rotation
		int dx = App->input->mouse_motion.x;
		int dy = App->input->mouse_motion.y;

		float3 front = direction_forward;
		float3 up = editor_camera->frustum->Up();
		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			float angle = -dy * SPEED_ROTATION * real_dt;
			Quat q = Quat::RotateAxisAngle(direction_right, angle);
			up = q * up;
			front = q * front;

			angle = -dx * SPEED_ROTATION * real_dt;
			q = Quat::RotateY(angle);
			up = q * up;
			front = q * front;
		}

		editor_camera->frustum->SetFrame(editor_camera->frustum->Pos() + translation, front, up);
	}
	
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	APPLOG("Releasing editor camera");

	RELEASE(editor_camera);

	return true;
}

void ModuleCamera::OnPlay()
{
	if (use_game_cameras)
	{
		game_camera = App->level->GetMainCamera();
		if (game_camera)
			rendering_camera = game_camera;
	}
}

void ModuleCamera::OnStop()
{
	game_camera = nullptr;
	rendering_camera = editor_camera;
}

void ModuleCamera::SetFOV(float fov)
{
	editor_camera->SetFOV(fov);
}

void ModuleCamera::SetAspectRatio(float aspect_ratio)
{
	editor_camera->SetAspectRatio(aspect_ratio);
}

void ModuleCamera::SetPlaneDistances(float nearPlaneDistance, float farPlaneDistance)
{
	editor_camera->SetPlaneDistances(nearPlaneDistance, farPlaneDistance);
}

void ModuleCamera::SetPosition(const float3& position)
{
	editor_camera->SetPosition(position);
}

void ModuleCamera::LookAt(const float3& position)
{
	editor_camera->LookAt(position);
}

float* ModuleCamera::GetProjectionMatrix() const
{
	return rendering_camera->GetProjectionMatrix();
}

float* ModuleCamera::GetViewMatrix() const 
{
	return rendering_camera->GetViewMatrix();
}

float3 ModuleCamera::GetPosition() const
{
	return rendering_camera->frustum->Pos();
}

bool ModuleCamera::InsideCulling(const AABB& box) const
{
	if (rendering_camera->frustum_culling)
		return rendering_camera->IsInsideFrustum(box);
	else
		return true;
}

void ModuleCamera::SetupFrustum(ComponentCamera* camera)
{
	camera->SetPlaneDistances(NEARPLANE, FARPLANE);
	camera->frustum->SetVerticalFovAndAspectRatio(DEG_TO_RAD * VERTICALFOV, ASPECTRATIO);
}
