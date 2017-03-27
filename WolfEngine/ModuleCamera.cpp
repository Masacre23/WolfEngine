#include "Math.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
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

	main_camera = editor_camera;

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
	float3 movement = float3::zero;
	float3 direction_forward = editor_camera->frustum->Front();
	float3 direction_right = editor_camera->frustum->WorldRight();
	float3 direction_up = float3::unitY;
	bool shift_pressed = (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT);

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += direction_forward;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement -= direction_forward;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) movement -= direction_right;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) movement += direction_right;
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) movement += direction_up;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) movement -= direction_up;
	
	movement += direction_forward * App->input->mouse_wheel.y * SPEED_ZOOM;

	editor_camera->frustum->Translate(movement * SPEED_TRANSLATION * (shift_pressed ? 2 : 1) * dt);

	// Camera rotation
	int dx = App->input->mouse_motion.x;
	int dy = App->input->mouse_motion.y;
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) 
	{
		float angle = -dy * SPEED_ROTATION * dt;
		Quat q = Quat::RotateAxisAngle(direction_right, angle);
		editor_camera->frustum->SetUp(q.Mul(editor_camera->frustum->Up()));
		editor_camera->frustum->SetFront(q.Mul(editor_camera->frustum->Front()));

		angle = - dx * SPEED_ROTATION * dt;
		q = Quat::RotateY(angle);
		editor_camera->frustum->SetUp(q.Mul(editor_camera->frustum->Up()));
		editor_camera->frustum->SetFront(q.Mul(editor_camera->frustum->Front()));

		
	}
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	APPLOG("Releasing editor camera");

	RELEASE(editor_camera);

	return true;
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
	return editor_camera->GetProjectionMatrix();
}

float* ModuleCamera::GetViewMatrix() const 
{
	return editor_camera->GetViewMatrix();
}

void ModuleCamera::SetupFrustum(ComponentCamera* camera)
{
	camera->SetPlaneDistances(NEARPLANE, FARPLANE);
	camera->SetFOV(DEG_TO_RAD * VERTICALFOV);
	camera->SetAspectRatio(ASPECTRATIO);
}
