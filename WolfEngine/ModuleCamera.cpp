#include "Math.h"
#include "ModuleCamera.h"

ModuleCamera::ModuleCamera() : Module(MODULE_CAMERA)
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Start()
{
	frustum = new Frustum();

	return true;
}

bool ModuleCamera::CleanUp()
{
	RELEASE(frustum);

	return true;
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


