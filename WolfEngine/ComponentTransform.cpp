#include "ComponentTransform.h"
#include "OpenGL.h"
#include "Interface.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Globals.h"

ComponentTransform::ComponentTransform(GameObject* parent) : Component(Component::Type::TRANSFORM, parent)
{
	rotation_euler = rotation.ToEulerXYZ().Abs();
	RecalculateLocalTransform();
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::OnDraw() const
{
	float* transform = global_transform.Transposed().ptr();
	glMultMatrixf(transform);
}

void ComponentTransform::OnDebugDraw() const
{
	float* transform = global_transform.Transposed().ptr();
	glMultMatrixf(transform);
}

bool ComponentTransform::OnEditor()
{
	bool node_open = ImGui::CollapsingHeader("Transform");
	float4x4 last_transform = local_transform;

	if (node_open)
	{
		ImGui::DragFloat3("Position", (float*)&position, 0.1f);

		float3 rot = rotation_euler * RAD_TO_DEG;
		ImGui::DragFloat3("Rotation", (float*)&rot, 1.0f, -180.0f, 180.0f);
		rotation_euler = rot * DEG_TO_RAD;
		rotation = Quat::FromEulerXYZ(rotation_euler[0], rotation_euler[1], rotation_euler[2]);

		ImGui::DragFloat3("Scale", (float*)&scale, 0.1f);

		RecalculateLocalTransform();

		transform_change = !(last_transform.Equals(local_transform));
	}

	return ImGui::IsItemClicked();
}

const float4x4& ComponentTransform::UpdateGlobalTransform(const float4x4& parent)
{
	global_transform = parent * local_transform;

	return global_transform;
}

void ComponentTransform::SetLocalTransform(const float3& position, const float3& scale, const Quat& rotation)
{
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	rotation_euler = rotation.ToEulerXYZ().Abs();

	RecalculateLocalTransform();
}

void ComponentTransform::SetLocalTransform(const float3& position, const Quat& rotation)
{
	this->position = position;
	this->rotation = rotation;
	rotation_euler = rotation.ToEulerXYZ().Abs();

	RecalculateLocalTransform();
}

void ComponentTransform::SetLocalTransform(const float3 & position)
{
	this->position = position;

	RecalculateLocalTransform();
}

void ComponentTransform::RecalculateLocalTransform()
{ 
	local_transform = float4x4::FromTRS(position, rotation, scale);
	transform_change = true;
}

void ComponentTransform::SaveComponent()
{ 
	backup_local_transform = local_transform;
	backup_rotatio_euler = rotation_euler;
}

void ComponentTransform::RestoreComponent()
{ 
	local_transform = backup_local_transform;
	position = local_transform.TranslatePart();
	rotation_euler = backup_rotatio_euler;
	rotation = Quat::FromEulerXYZ(rotation_euler[0], rotation_euler[1], rotation_euler[2]);
	scale = local_transform.GetScale();
	transform_change = true;
}
