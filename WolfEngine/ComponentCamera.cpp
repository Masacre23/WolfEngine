#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ComponentCamera.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "OpenGL.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "Color.h"

ComponentCamera::ComponentCamera(GameObject* parent) : Component(Component::Type::CAMERA, parent)
{
	frustum = new Frustum();

	frustum->SetPos(float3::zero);
	frustum->SetFront(float3::unitZ);
	frustum->SetUp(float3::unitY);

	App->camera->SetupFrustum(this);
}

ComponentCamera::~ComponentCamera()
{
	RELEASE(frustum);
}

bool ComponentCamera::OnUpdate()
{
	if (parent != nullptr)
	{
		SetPosition(parent->transform->GetPosition());
		SetOrientation(parent->transform->GetRotation());
	}		

	return true;
}

bool ComponentCamera::OnDraw() const
{
	App->renderer->DrawFrustum(*frustum, Colors::Yellow);

	return true;
}

bool ComponentCamera::OnEditor()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Checkbox("Active", &enable);

		if (ImGui::Button("Default camera configuration"))
			App->camera->SetupFrustum(this);

		float near_plane = frustum->NearPlaneDistance();
		if (ImGui::DragFloat("Near Plane", &near_plane, 0.1f, 0.1f, 1000.0f))
		{
			if (near_plane >= 0.1f && near_plane < frustum->FarPlaneDistance())
				SetPlaneDistances(near_plane, frustum->FarPlaneDistance());
		}

		float far_plane = frustum->FarPlaneDistance();
		if (ImGui::DragFloat("Far Plane", &far_plane, 1.0f, 10.0f, 10000.0f))
		{
			if (far_plane >= 10.0f && far_plane > frustum->NearPlaneDistance())
				SetPlaneDistances(frustum->NearPlaneDistance(), far_plane);
		}

		float vertical_fov = frustum->VerticalFov() * RAD_TO_DEG;
		if (ImGui::SliderFloat("Vertical FOV", &vertical_fov, 1.0f, 180.0f))
			SetFOV(vertical_fov);

		float aspect_ratio = frustum->AspectRatio();
		if (ImGui::DragFloat("Aspect Ratio", &aspect_ratio, 0.1f, 0.1f, 1000.0f))
			SetAspectRatio(aspect_ratio);
	}

	return ImGui::IsItemClicked();
}

void ComponentCamera::SetFOV(float fov)
{
	frustum->SetVerticalFovAndAspectRatio(DEG_TO_RAD * fov, frustum->AspectRatio());
}

void ComponentCamera::SetAspectRatio(float aspect_ratio)
{
	float fov = frustum->VerticalFov();
	float horizontalFov = 2.0f * atanf(tanf(fov / 2.0f) * aspect_ratio);
	frustum->SetHorizontalFovAndAspectRatio(horizontalFov, aspect_ratio);
}

void ComponentCamera::SetPlaneDistances(float nearPlaneDistance, float farPlaneDistance)
{
	frustum->SetViewPlaneDistances(nearPlaneDistance, farPlaneDistance);
}

void ComponentCamera::SetPosition(const float3& position)
{
	frustum->SetPos(position);
}

void ComponentCamera::SetOrientation(const Quat& rotation)
{
	frustum->SetFront(rotation.Mul(float3::unitZ));
	frustum->SetUp(rotation.Mul(float3::unitY));
}

void ComponentCamera::LookAt(const float3 & position)
{
	float3 direction = position - frustum->Pos();

	float3x3 matrix = float3x3::LookAt(frustum->Front(), direction.Normalized(), frustum->Up(), float3::unitY);

	frustum->SetFront(matrix.MulDir(frustum->Front()).Normalized());
	frustum->SetUp(matrix.MulDir(frustum->Up()).Normalized());
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
