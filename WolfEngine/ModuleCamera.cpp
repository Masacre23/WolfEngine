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
	frustum->type = FrustumType::PerspectiveFrustum;
	SetPosition({ 2.0f, 4.0f, 5.0f });
	frustum->front = float3(0, 0, -1);
	frustum->up = float3(0, 1, 0);
	SetPlaneDistances(0.1f, 5000.0f);
	
	frustum->verticalFov = 59.0f;
	frustum->horizontalFov = 90.0f;

	return true;
}

update_status ModuleCamera::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		SetPosition(frustum->pos + speed * dt * frustum->front);
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		SetPosition(frustum->pos -speed * dt * frustum->front);
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		SetPosition(frustum->pos -speed * dt * frustum->WorldRight());
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		SetPosition(frustum->pos + speed * dt * frustum->WorldRight());
	}

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		frustum->pos.y += speed * dt;

	if(App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
		frustum->pos.y -= speed * dt;

	// Camera rotation

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		Quat q = { 0,-0.008726535498373935f,0,0.9999619230641713f }; // 1 degree
		float3 u = {q.x, q.y, q.z};
		float s = q.w;
		frustum->up = 2.0f * Dot(u, frustum->up) * u + (s * s - Dot(u, u)) * frustum->up + 2.0f * s * Cross(u, frustum->up);
		frustum->front = 2.0f * Dot(u, frustum->front) * u + (s * s - Dot(u, u)) * frustum->front + 2.0f * s * Cross(u, frustum->front);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		Quat q = { 0,0.008726535498373935f,0,0.9999619230641713f }; // 1 degree
		float3 u = { q.x, q.y, q.z };
		float s = q.w;
		frustum->up = 2.0f * Dot(u, frustum->up) * u + (s * s - Dot(u, u)) * frustum->up + 2.0f * s * Cross(u, frustum->up);
		frustum->front = 2.0f * Dot(u, frustum->front) * u + (s * s - Dot(u, u)) * frustum->front + 2.0f * s * Cross(u, frustum->front);
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		Quat q = { -0.008726535498373935f,0,0,0.9999619230641713f }; // 1 degree
		float3 u = { q.x, q.y, q.z };
		float s = q.w;
		frustum->up = 2.0f * Dot(u, frustum->up) * u + (s * s - Dot(u, u)) * frustum->up + 2.0f * s * Cross(u, frustum->up);
		frustum->front = 2.0f * Dot(u, frustum->front) * u + (s * s - Dot(u, u)) * frustum->front + 2.0f * s * Cross(u, frustum->front);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		Quat q = { 0.008726535498373935f,0,0,0.9999619230641713f }; // 1 degree
		float3 u = { q.x, q.y, q.z };
		float s = q.w;
		frustum->up = 2.0f * Dot(u, frustum->up) * u + (s * s - Dot(u, u)) * frustum->up + 2.0f * s * Cross(u, frustum->up);
		frustum->front = 2.0f * Dot(u, frustum->front) * u + (s * s - Dot(u, u)) * frustum->front + 2.0f * s * Cross(u, frustum->front);
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


