#include "ComponentTransform.h"
#include "OpenGL.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "Application.h"
#include "ModuleWindow.h"

ComponentTransform::ComponentTransform(GameObject* parent) : Component(Component::Type::TRANSFORM, parent)
{
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Load(const float3& position, const float3& scale, const Quat& rotation)
{
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
}

bool ComponentTransform::OnUpdate()
{
	return true;
}

bool ComponentTransform::OnDraw() const
{
	float* transform = float4x4::FromTRS(position, rotation, scale).Transposed().ptr();
	glMultMatrixf(transform);

	return true;
}

bool ComponentTransform::OnEditor(int selection_mask, int id)
{
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << id)) ? ImGuiTreeNodeFlags_Selected : 0);
	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)id, node_flags, "Transform");
	
	if (node_open)
	{
		float pos[3] = {position.x, position.y, position.z};
		ImGui::DragFloat3("Position", pos, 0.1f);
		position = float3(pos[0], pos[1], pos[2]);

		float rot[4] = { rotation.x, rotation.y, rotation.z, rotation.w };
		ImGui::DragFloat4("Rotation", rot, 0.1f);
		rotation = Quat(rot[0], rot[1], rot[2], rot[3]);

		float sca[3] = { scale.x, scale.y, scale.z };
		ImGui::DragFloat3("Scale", sca, 0.1f, 0.0f);
		scale = float3(sca[0], sca[1], sca[2]);

		ImGui::TreePop();
	}

	return ImGui::IsItemClicked();
}
