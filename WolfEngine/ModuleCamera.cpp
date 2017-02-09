#include "Math.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "Application.h"

#include "ModuleWindow.h"
#include "SDL/include/SDL.h"

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
	//SDL_WarpMouseGlobal(App->window->GetScreenWidth() / 2, App->window->GetScreenHeight() / 2);

	SDL_ShowCursor(0);
	int x = 0, y = 0;
	//SDL_GetMouseState(&x, &y);
	//int dx = x - App->window->GetScreenWidth()/2;
	//int dy = y - App->window->GetScreenHeight()/2;
	int dx = App->input->mouse_motion.x;
	int dy = App->input->mouse_motion.y;

	if (dx != 0)
	{
		Quat q;
		static float last_positionX;
		
		if(App->input->mouse_position.x > last_positionX)
		    q = { 0,-0.008726535498373935f,0,0.9999619230641713f}; // 1 degree
		else
			q = { 0,0.008726535498373935f,0,0.9999619230641713f};

		last_positionX = App->input->mouse_position.x;

		float3 u = { q.x, q.y, q.z };
		float s = q.w;
		frustum->up = 2.0f * Dot(u, frustum->up) * u + (s * s - Dot(u, u)) * frustum->up + 2.0f * s * Cross(u, frustum->up);
		frustum->front = 2.0f * Dot(u, frustum->front) * u + (s * s - Dot(u, u)) * frustum->front + 2.0f * s * Cross(u, frustum->front);
	}

	if (dy != 0)
	{
		static float last_positionY;

		float module = Sqrt(frustum->front.x * frustum->front.x + frustum->front.y * frustum->front.y + frustum->front.z* frustum->front.z);
		float alfa = frustum->WorldRight().x / module;
		float beta = frustum->WorldRight().y / module;
		float tecta = frustum->WorldRight().z / module;

		Quat q = { -0.008726535498373935f,0,0,0.9999619230641713f }; // 1 degree
		if (App->input->mouse_position.y < last_positionY)
		{
			q.w = Cos((pi / 360) / 2);
			q.x = Sin((pi / 360) / 2)*alfa;
			q.y = Sin((pi / 360) / 2)*beta;
			q.z = Sin((pi / 360) / 2) * tecta;
		}
		else
		{
			q.w = Cos((pi / 360) / 2);
			q.x = -Sin((pi / 360) / 2)*alfa;
			q.y = -Sin((pi / 360) / 2)*beta;
			q.z = -Sin((pi / 360) / 2) * tecta;
		}

		last_positionY = App->input->mouse_position.y;

		float3 u = { q.x, q.y, q.z };
		float s = q.w;
		frustum->up = 2.0f * Dot(u, frustum->up) * u + (s * s - Dot(u, u)) * frustum->up + 2.0f * s * Cross(u, frustum->up);
		frustum->front = 2.0f * Dot(u, frustum->front) * u + (s * s - Dot(u, u)) * frustum->front + 2.0f * s * Cross(u, frustum->front);
	}
	/*if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
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
	}*/

	/*if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		float module = Sqrt(frustum->front.x * frustum->front.x + frustum->front.y * frustum->front.y + frustum->front.z* frustum->front.z);
		float alfa = frustum->WorldRight().x / module;
		float beta = frustum->WorldRight().y / module;
		float tecta = frustum->WorldRight().z / module;

		Quat q = { -0.008726535498373935f,0,0,0.9999619230641713f }; // 1 degree
		q.w = Cos((pi / 360)/2);
		q.x = Sin((pi / 360) / 2)*alfa;
		q.y = Sin((pi / 360) / 2)*beta;
		q.z = Sin((pi / 360) / 2) * tecta;

		float3 u = { q.x, q.y, q.z };
		float s = q.w;
		frustum->up = 2.0f * Dot(u, frustum->up) * u + (s * s - Dot(u, u)) * frustum->up + 2.0f * s * Cross(u, frustum->up);
		frustum->front = 2.0f * Dot(u, frustum->front) * u + (s * s - Dot(u, u)) * frustum->front + 2.0f * s * Cross(u, frustum->front);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		float module = Sqrt(frustum->front.x * frustum->front.x + frustum->front.y * frustum->front.y + frustum->front.z* frustum->front.z);
		float alfa = frustum->WorldRight().x / module;
		float beta = frustum->WorldRight().y / module;
		float tecta = frustum->WorldRight().z / module;

		Quat q = { 0.008726535498373935f,0,0,0.9999619230641713f }; // 1 degree
		q.w = Cos((pi / 360) / 2);
		q.x = -Sin((pi / 360) / 2)*alfa;
		q.y = -Sin((pi / 360) / 2)*beta;
		q.z = -Sin((pi / 360) / 2) * tecta;
		float3 u = { q.x, q.y, q.z };
		float s = q.w;
		frustum->up = 2.0f * Dot(u, frustum->up) * u + (s * s - Dot(u, u)) * frustum->up + 2.0f * s * Cross(u, frustum->up);
		frustum->front = 2.0f * Dot(u, frustum->front) * u + (s * s - Dot(u, u)) * frustum->front + 2.0f * s * Cross(u, frustum->front);
	}*/
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


