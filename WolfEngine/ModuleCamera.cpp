#include "Math.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleWindow.h"
#include "SDL/include/SDL.h"
#include "JsonHandler.h"

ModuleCamera::ModuleCamera() : Module(MODULE_CAMERA)
{
	frustum = new Frustum();

	frustum->type = FrustumType::PerspectiveFrustum;
	frustum->pos = float3::zero;
	frustum->front = float3::unitZ;
	frustum->up = float3::unitY;

	frustum->nearPlaneDistance = 0.1f;
	frustum->farPlaneDistance = 5000.0f;
	frustum->verticalFov = DEG_TO_RAD * 59.0f;
	SetAspectRatio(1.5f);

}

ModuleCamera::~ModuleCamera()
{
	RELEASE(frustum);
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

	SetPosition(initial_pos);
	LookAt(float3::zero);

	return true;
}

update_status ModuleCamera::Update(float dt)
{
	float3 movement = float3::zero;
	float3 direction_forward = frustum->front;
	float3 direction_right = frustum->WorldRight();
	float3 direction_up = float3::unitY;
	bool shift_pressed = (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT);

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += direction_forward;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement -= direction_forward;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) movement -= direction_right;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) movement += direction_right;
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) movement += direction_up;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) movement -= direction_up;
	
	movement += direction_forward * App->input->mouse_wheel.y * extra_speed_zoom;

	frustum->Translate(movement * speed_translation * (shift_pressed ? 2 : 1) * dt);

	// Camera rotation
	int dx = App->input->mouse_motion.x;
	int dy = App->input->mouse_motion.y;
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) 
	{
		float angle = -dy * speed_rotation * dt;
		Quat q = Quat::RotateAxisAngle(direction_right, angle);
		frustum->up = q.Mul(frustum->up);
		frustum->front = q.Mul(frustum->front);

		angle = - dx * speed_rotation * dt;
		q = Quat::RotateY(angle);
		frustum->up = q.Mul(frustum->up);
		frustum->front = q.Mul(frustum->front);

		
	}
	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::SetFOV(float fov)
{
	float r = frustum->AspectRatio();
	frustum->verticalFov = DEG_TO_RAD * fov;
	SetAspectRatio(r);
}

void ModuleCamera::SetAspectRatio(float aspect_ratio)
{
	float fov = frustum->verticalFov;
	frustum->horizontalFov = 2.0f * atanf(tanf(fov / 2.0f) * aspect_ratio);
}

void ModuleCamera::SetPlaneDistances(float nearPlaneDistance, float farPlaneDistance)
{
	frustum->nearPlaneDistance = nearPlaneDistance;
	frustum->farPlaneDistance = farPlaneDistance;
}

void ModuleCamera::SetPosition(const float3& position)
{
	frustum->pos = position;
}

void ModuleCamera::SetOrientation(const float3& rotation)
{
	frustum->front = float3::unitZ;
	frustum->up = float3::unitY;
}

void ModuleCamera::LookAt(const float3& position)
{
	float3 direction = position - frustum->pos;

	float3x3 matrix = float3x3::LookAt(frustum->front, direction.Normalized(), frustum->up, float3::unitY);

	frustum->front = matrix.MulDir(frustum->front).Normalized();
	frustum->up = matrix.MulDir(frustum->up).Normalized();
}

float* ModuleCamera::GetProjectionMatrix() const
{
	float4x4 matrix = frustum->ProjectionMatrix();

	return (float*)matrix.Transposed().v;
}

float* ModuleCamera::GetViewMatrix() const 
{
	float4x4 matrix = (float4x4)frustum->ViewMatrix();

	return (float*)matrix.Transposed().v;
}


