#include "ComponentTransform.h"
#include "OpenGL.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
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

const float4x4& ComponentTransform::UpdateTransform(const float4x4& parent)
{
	global_transform = parent * local_transform;

	return global_transform;
}

void ComponentTransform::Load(const float3& position, const float3& scale, const Quat& rotation)
{
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	rotation_euler = rotation.ToEulerXYZ().Abs();

	RecalculateLocalTransform();
}

void ComponentTransform::Load(const float3 & position, const Quat & rotation)
{
	this->position = position;
	this->rotation = rotation;
	rotation_euler = rotation.ToEulerXYZ().Abs();

	RecalculateLocalTransform();
}

bool ComponentTransform::OnUpdate()
{
	return true;
}

bool ComponentTransform::OnDraw() const
{
	float* transform = global_transform.Transposed().ptr();
	glMultMatrixf(transform);
	
	return true;
}

bool ComponentTransform::OnEditor()
{
	//ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << id)) ? ImGuiTreeNodeFlags_Selected : 0);
	//bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)id, node_flags, "Transform");
	bool node_open = ImGui::CollapsingHeader("Transform");
	
	if (node_open)
	{
		ImGui::DragFloat3("Position", (float*)&position, 0.1f);

		float3 rot = rotation_euler * RAD_TO_DEG;
		ImGui::DragFloat3("Rotation", (float*)&rot, 1.0f, -180.0f, 180.0f);
		rotation_euler = rot * DEG_TO_RAD;
		rotation = Quat::FromEulerXYZ(rotation_euler[0], rotation_euler[1], rotation_euler[2]);

		ImGui::DragFloat3("Scale", (float*)&scale, 0.1f);

		RecalculateLocalTransform();

		//ImGui::TreePop();
	}

	return ImGui::IsItemClicked();
}

void ComponentTransform::RecalculateLocalTransform()
{ 
	local_transform = float4x4::FromTRS(position, rotation, scale); 
}
