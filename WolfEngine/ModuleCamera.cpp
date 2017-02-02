#include "Math.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "Application.h"

ModuleCamera::ModuleCamera() : Module(MODULE_CAMERA)
{
	frustum = new Frustum();
}

ModuleCamera::~ModuleCamera()
{
	RELEASE(frustum);
}

bool ModuleCamera::Start()
{
	frustum = new Frustum();

	return true;
}

update_status ModuleCamera::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		frustum->pos.y += dt;

	if(App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
		frustum->pos.y -= dt;

	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	RELEASE(frustum);

	return true;
}

void ModuleCamera::SetFOV(float fov)
{
	float r = frustum->AspectRatio();
	frustum->verticalFov = fov;
	SetFOH(fov, r);
}

void ModuleCamera::SetAspectRatio(float r)
{
	float fov = frustum->verticalFov;
	SetFOH(fov, r);
}

void ModuleCamera::SetPlaneDistances(float nearPlaneDistance, float farPlaneDistance)
{
	frustum->nearPlaneDistance = nearPlaneDistance;
	frustum->farPlaneDistance = farPlaneDistance;
}

void ModuleCamera::SetPosition(float3 position)
{
	frustum->pos = position;
}

void ModuleCamera::SetOrientation(float3 rotation)
{
	
}

void ModuleCamera::LookAt(float3 position)
{
	frustum->front = frustum->pos - position;
}

float* ModuleCamera::GetProjectionMatrix()
{
	float* ret = &(frustum->ProjectionMatrix().v[0][0]);
	return ret;
}

float* ModuleCamera::GetViewMatrix()
{
	float* ret = &(frustum->ViewMatrix().v[0][0]);
	return ret;
}

void ModuleCamera::WindowResize(int width, int height)
{
	float fov = 2 * Atan(width);
	float foh = 2 * Atan(height);
	frustum->verticalFov = fov;
	frustum->horizontalFov = foh;
}

void ModuleCamera::SetFOH(float fov, float r)
{
	float foh = r * fov;
	frustum->horizontalFov = foh;
}


