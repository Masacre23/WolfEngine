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
	editor_camera = new ComponentCamera();

	editor_camera->frustum->type = FrustumType::PerspectiveFrustum;
	editor_camera->frustum->pos = float3::zero;
	editor_camera->frustum->front = float3::unitZ;
	editor_camera->frustum->up = float3::unitY;

	editor_camera->frustum->nearPlaneDistance = 0.1f;
	editor_camera->frustum->farPlaneDistance = 5000.0f;
	editor_camera->frustum->verticalFov = DEG_TO_RAD * 59.0f;
	editor_camera->SetAspectRatio(1.5f);
}

ModuleCamera::~ModuleCamera()
{
	RELEASE(editor_camera);
}

bool ModuleCamera::Start()
{
	float3 initial_pos = float3::zero;
	if (App->parser->LoadObject(CAMERA_SECTION))
	{
		speed_rotation = App->parser->GetFloat("RotationSpeed");
		speed_translation = App->parser->GetFloat("TranslationSpeed");
		extra_speed_zoom = App->parser->GetFloat("ZoomSpeedFactor");
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
	float3 direction_forward = editor_camera->frustum->front;
	float3 direction_right = editor_camera->frustum->WorldRight();
	float3 direction_up = float3::unitY;
	bool shift_pressed = (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT);

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += direction_forward;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement -= direction_forward;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) movement -= direction_right;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) movement += direction_right;
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) movement += direction_up;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) movement -= direction_up;
	
	movement += direction_forward * App->input->mouse_wheel.y * extra_speed_zoom;

	editor_camera->frustum->Translate(movement * speed_translation * (shift_pressed ? 2 : 1) * dt);

	// Camera rotation
	int dx = App->input->mouse_motion.x;
	int dy = App->input->mouse_motion.y;
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) 
	{
		float angle = -dy * speed_rotation * dt;
		Quat q = Quat::RotateAxisAngle(direction_right, angle);
		editor_camera->frustum->up = q.Mul(editor_camera->frustum->up);
		editor_camera->frustum->front = q.Mul(editor_camera->frustum->front);

		angle = - dx * speed_rotation * dt;
		q = Quat::RotateY(angle);
		editor_camera->frustum->up = q.Mul(editor_camera->frustum->up);
		editor_camera->frustum->front = q.Mul(editor_camera->frustum->front);

		
	}
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
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


