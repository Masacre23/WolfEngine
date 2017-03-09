#include "ComponentTransform.h"
#include "OpenGL.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Globals.h"

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
	//glTranslatef(position.x, position.y, position.z);

	return true;
}

bool ComponentTransform::OnEditor()
{
	bool b = true;
	static int selection_mask = (1 << 2);
	int node_clicked = -1;

	ImGui::SetNextWindowPos(ImVec2(App->window->GetScreenWidth() - App->window->GetScreenWidth() / 5, 20));
	ImGui::SetNextWindowSize(ImVec2(App->window->GetScreenWidth() / 5, App->window->GetScreenHeight() - App->window->GetScreenHeight() / 3 - 20));
	ImGui::Begin("Inspector", &b, ImVec2(App->window->GetScreenWidth() / 5, App->window->GetScreenHeight() / 1.58f), -1.0f, ImGuiWindowFlags_ChildWindowAutoFitX | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_ChildWindowAutoFitY | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	node_clicked = -1;
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.
	
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << 0)) ? ImGuiTreeNodeFlags_Selected : 0);
	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)0, node_flags, "Transform");
	if (ImGui::IsItemClicked())
		node_clicked = 0;
	
	if (node_open)
	{
		float pos[3] = {position.x, position.y, position.z};
		ImGui::DragFloat3("Position", pos, 0.1f);
		position = float3(pos[0], pos[1], pos[2]);

		float3 rotation_euler = rotation.ToEulerXYZ();
		float rot[3] = { rotation_euler.x, rotation_euler.y, rotation_euler.z };
		ImGui::DragFloat3("Rotation", rot, 0.1f);
		rotation = Quat::RotateX(rot[0]).Mul(Quat::RotateY(rot[1])).Mul(Quat::RotateZ(rot[2]));

		float sca[3] = { scale.x, scale.y, scale.z };
		ImGui::DragFloat3("Scale", sca, 0.1f);
		scale = float3(sca[0], sca[1], sca[2]);

		ImGui::TreePop();
	}
	
	if (node_clicked != -1)
	{
		// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
		if (ImGui::GetIO().KeyCtrl)
			selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
			selection_mask = (1 << node_clicked);           // Click to single-select
	}

	ImGui::PopStyleVar();
	ImGui::End();

	return false;
}
