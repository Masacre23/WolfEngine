#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleLevel.h"
#include "ComponentCamera.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "OpenGL.h"
#include "Interface.h"
#include "Color.h"

ComponentCamera::ComponentCamera(GameObject* parent) : Component(Component::Type::CAMERA, parent)
{
	frustum = new Frustum();

	frustum->SetPos(float3::zero);
	frustum->SetFront(float3::unitZ);
	frustum->SetUp(float3::unitY);

	frustum->SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);

	SetAABB();
	App->camera->SetupFrustum(this);
}

ComponentCamera::~ComponentCamera()
{
	RELEASE(frustum);
}

void ComponentCamera::OnUpdate()
{
	if (parent != nullptr)
	{
		SetPosition(parent->transform->GetPosition());
		SetOrientation(parent->transform->GetRotation());
	}		
}

void ComponentCamera::OnDebugDraw() const
{
	App->renderer->debug_drawer->DrawFrustum(*frustum, Colors::Yellow);
}

bool ComponentCamera::OnEditor()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		//ImGui::Checkbox("Active##Camera", &enable);

		if (App->level->GetMainCamera() == this)
		{
			ImGui::Text("Main Camera");
		}
		else
		{
			if (ImGui::Button("Set as Main Camera"))
				App->level->SetMainCamera(this);
		}

		ImGui::Checkbox("Frustum culling", &frustum_culling);

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
	SetAABB();
}

void ComponentCamera::SetAspectRatio(float aspect_ratio)
{
	float fov = frustum->VerticalFov();
	float horizontalFov = 2.0f * atanf(tanf(fov / 2.0f) * aspect_ratio);
	frustum->SetHorizontalFovAndAspectRatio(horizontalFov, aspect_ratio);
	SetAABB();
}

void ComponentCamera::SetPlaneDistances(float nearPlaneDistance, float farPlaneDistance)
{
	frustum->SetViewPlaneDistances(nearPlaneDistance, farPlaneDistance);
	SetAABB();
}

void ComponentCamera::SetPosition(const float3& position)
{
	frustum->SetPos(position);
	SetAABB();
}

void ComponentCamera::SetOrientation(const Quat& rotation)
{
	frustum->SetFront(rotation.Mul(float3::unitZ));
	frustum->SetUp(rotation.Mul(float3::unitY));
	SetAABB();
}

void ComponentCamera::LookAt(const float3 & position)
{
	float3 direction = position - frustum->Pos();

	float3x3 matrix = float3x3::LookAt(frustum->Front(), direction.Normalized(), frustum->Up(), float3::unitY);

	frustum->SetFront(matrix.MulDir(frustum->Front()).Normalized());
	frustum->SetUp(matrix.MulDir(frustum->Up()).Normalized());
	SetAABB();
}

bool ComponentCamera::IsInsideFrustum(const AABB& box) const
{
	float3 corners[8];
	box.GetCornerPoints(corners);

	Plane planes[6];
	frustum->GetPlanes(planes);

	for (int i = 0; i < 6; i++)
	{
		int out = 0;
		for (int j = 0; j < 8; j++)
			out += planes[i].IsOnPositiveSide(corners[j]);
		if (out == 8)
			return false;
	}
	return true;
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

void ComponentCamera::SetAABB() const
{
	if (parent != nullptr)
	{
		parent->initial_bbox.SetNegativeInfinity();
		parent->initial_bbox.Enclose(*frustum);
		parent->bbox = parent->initial_bbox;
	}
}
