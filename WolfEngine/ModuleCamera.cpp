#include "Math.h"
#include "ModuleCamera.h"

ModuleCamera::ModuleCamera() : Module(MODULE_CAMERA)
{
}

ModuleCamera::~ModuleCamera()
{
}

void ModuleCamera::SetFOV()
{
}

void ModuleCamera::SetAspectRatio()
{
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


