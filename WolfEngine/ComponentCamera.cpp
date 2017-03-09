#include "ComponentCamera.h"

ComponentCamera::ComponentCamera(GameObject* parent) : Component(Component::Type::CAMERA, parent)
{
	frustum = new Frustum();
}

ComponentCamera::~ComponentCamera()
{
	RELEASE(frustum);
}

void ComponentCamera::SetFOV(float fov)
{
	float r = frustum->AspectRatio();
	frustum->verticalFov = DEG_TO_RAD * fov;
	SetAspectRatio(r);
}

void ComponentCamera::SetAspectRatio(float aspect_ratio)
{
	float fov = frustum->verticalFov;
	frustum->horizontalFov = 2.0f * atanf(tanf(fov / 2.0f) * aspect_ratio);
}

void ComponentCamera::SetPlaneDistances(float nearPlaneDistance, float farPlaneDistance)
{
	frustum->nearPlaneDistance = nearPlaneDistance;
	frustum->farPlaneDistance = farPlaneDistance;
}

void ComponentCamera::SetPosition(const float3 & position)
{
	frustum->pos = position;
}

void ComponentCamera::LookAt(const float3 & position)
{
	float3 direction = position - frustum->pos;

	float3x3 matrix = float3x3::LookAt(frustum->front, direction.Normalized(), frustum->up, float3::unitY);

	frustum->front = matrix.MulDir(frustum->front).Normalized();
	frustum->up = matrix.MulDir(frustum->up).Normalized();
}

float* ComponentCamera::GetProjectionMatrix() const
{
	float4x4 matrix = frustum->ProjectionMatrix();

	return (float*)matrix.Transposed().v;
}

float* ComponentCamera::GetViewMatrix() const
{
	float4x4 matrix = (float4x4)frustum->ViewMatrix();

	return (float*)matrix.Transposed().v;
}
