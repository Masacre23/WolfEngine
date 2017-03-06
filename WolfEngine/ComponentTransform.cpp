#include "ComponentTransform.h"
#include "OpenGL.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "Application.h"
#include "ModuleWindow.h"

ComponentTransform::ComponentTransform(GameObject* parent) : Component(Component::Type::TRANSFORM, parent)
{
	//position = float3(0, 0, 0);
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
	float* transform = (float*)float4x4::FromTRS(position, rotation, scale).v;
	glMultMatrixf(transform);
	//glTranslatef(position.x, position.y, position.z);

	return true;
}

bool ComponentTransform::OnEditor()
{
	bool* b = new bool(true);
	static int selection_mask = (1 << 2);
	int node_clicked = -1;

	ImGui::SetNextWindowPos(ImVec2(App->window->GetScreenWidth() - App->window->GetScreenWidth() / 5, 20));
	ImGui::SetNextWindowSize(ImVec2(App->window->GetScreenWidth() / 5, App->window->GetScreenHeight() - App->window->GetScreenHeight() / 3 - 20));
	ImGui::Begin("Inspector", b, ImVec2(App->window->GetScreenWidth() / 5, App->window->GetScreenHeight() / 1.58f), -1.0f, ImGuiWindowFlags_ChildWindowAutoFitX | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_ChildWindowAutoFitY | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	node_clicked = -1;
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.
	for (int i = 0; i < 1; i++)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);
		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Transform");
		if (ImGui::IsItemClicked())
			node_clicked = i;

		if (node_open)
		{
			float pos[3] = {position.x, position.y, position.z};
			ImGui::DragFloat3("Position", pos, 1.0f);
			
			position = float3(pos[0], pos[1], pos[2]);
			LOG("%f", position.x);

			ImGui::SameLine();
			ImGui::TreePop();
		}
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
