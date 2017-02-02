#include "Math.h"
#include "ModuleCamera.h"

ModuleCamera::ModuleCamera() : Module(MODULE_CAMERA)
{
	frustum = new Frustum();
}

ModuleCamera::~ModuleCamera()
{
	RELEASE(frustum);
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

float4x4 ModuleCamera::GetProjectionMatrix()
{
	float4x4 ret;

	return ret;
}

float4x4 ModuleCamera::GetViewMatrix()
{
	float4x4 ret;

	return ret;
}

void ModuleCamera::SetFOH(float fov, float r)
{
	float foh = 2 * Atan(r * tan(fov / 2));
	frustum->horizontalFov = foh;
}


