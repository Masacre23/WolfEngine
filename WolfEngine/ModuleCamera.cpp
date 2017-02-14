#include "Math.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleWindow.h"
#include "SDL/include/SDL.h"

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
	SetPosition({ -1.0f, 2.0f, 4.0f });
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
	
	movement += direction_forward * App->input->mouse_wheel.y * 5.0f;

	frustum->Translate(movement * translation_speed * (shift_pressed ? 2 : 1) * dt);

	// Camera rotation
	//SDL_WarpMouseGlobal(App->window->GetScreenWidth() / 2, App->window->GetScreenHeight() / 2);

	SDL_ShowCursor(0);
	int x = 0, y = 0;
	//SDL_GetMouseState(&x, &y);
	//int dx = x - App->window->GetScreenWidth()/2;
	//int dy = y - App->window->GetScreenHeight()/2;
	int dx = App->input->mouse_motion.x;
	int dy = App->input->mouse_motion.y;
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
		if (dx != 0)
		{
			Quat q;
			static float last_positionX;

			if (App->input->mouse_position.x > last_positionX)
				q = { 0,-0.008726535498373935f,0,0.9999619230641713f }; // 1 degree
			else
				q = { 0,0.008726535498373935f,0,0.9999619230641713f };
	
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


