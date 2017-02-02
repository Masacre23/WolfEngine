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

bool ModuleCamera::Start()
{
	frustum->type = FrustumType::PerspectiveFrustum;
	SetPosition({ 2.0f, 4.0f, 5.0f });
	frustum->front = float3(0, 0, -1);
	frustum->up = float3(0, 1, 0);
	SetPlaneDistances(0.1f, 5000.0f);
	
	frustum->verticalFov = 59.0f;
	frustum->horizontalFov = 90.0f;

	return true;
}

bool ModuleCamera::CleanUp()
{
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
	frustum->front = float3(0, 0, 1);
	frustum->up = float3(0, 1, 0);
}

void ModuleCamera::LookAt(float3 position)
{
	frustum->front = frustum->pos - position;
}

float* ModuleCamera::GetProjectionMatrix()
{
	float* ret = &(frustum->ProjectionMatrix().Transposed().v[0][0]);
	return ret;
}

float* ModuleCamera::GetViewMatrix()
{
	float4x4 view_matrix = frustum->ViewMatrix();
	float* ret = &(view_matrix.Transposed().v[0][0]);
	return ret;
}

void ModuleCamera::WindowResize(int width, int height)
{
	float r = width / height;
	float fov = frustum->verticalFov;
	//SetFOH(fov, r);
}

void ModuleCamera::SetFOH(float fov, float r)
{
	float foh = 2 * Atan(r * tan(fov / 2));
	frustum->horizontalFov = foh;
}


