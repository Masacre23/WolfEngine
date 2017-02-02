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

void ModuleCamera::SetPlaneDistances()
{
}

void ModuleCamera::SetPosition()
{
}

void ModuleCamera::SetOrientation()
{
}

void ModuleCamera::LookAt(float3 position)
{
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


