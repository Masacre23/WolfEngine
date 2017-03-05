#include "PanelHierachy.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleLevel.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Component.h"

PanelHierachy::PanelHierachy() : Panel("Hierachy")
{
	active = new bool(true);
}

PanelHierachy::~PanelHierachy()
{}

void PanelHierachy::Draw(std::vector<GameObject*> game_objects)
{
	bool* b = new bool(true);

	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(App->window->GetScreenWidth() / 5, App->window->GetScreenHeight() - App->window->GetScreenHeight() / 3 - 20));

	ImGui::Begin("Hierachy", b, ImVec2(App->window->GetScreenWidth() / 5, App->window->GetScreenHeight() / 1.58f), -1.0f, ImGuiWindowFlags_ChildWindowAutoFitX | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_ChildWindowAutoFitY | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	//ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
	ImGui::Unindent(15.0f);

	static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
	static int selection_mask_child = (1 << 2);
	static int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
	static int child_clicked = -1;
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.
	for (int i = 0; i < game_objects.size(); i++)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);
		//if (game_objects[i] == "Root")
		//{
			// Node
			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, game_objects[i]->name.c_str());
			if (ImGui::IsItemClicked())
				node_clicked = i;
			if (node_open)
			{
				for (int j = 0; j < game_objects[i]->childs.size(); ++j)
				{
					node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask_child & (1 << j)) ? ImGuiTreeNodeFlags_Selected : 0);
					ImGui::TreeNodeEx((void*)(intptr_t)(j + i), node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, game_objects[i]->childs[j]->name.c_str());
					if (ImGui::IsItemClicked())
					{
						Component* c = new Component(*game_objects[i]->childs[j]->GetComponent(Component::Type::TRANSFORM));
						ComponentTransform* ct = (ComponentTransform*)(c);
						ct->OnEditor();
						child_clicked = j;
					}
				}
				ImGui::TreePop();
			}
		/*}
		else
		{
			// Leaf: The only reason we have a TreeNode at all is to allow selection of the leaf. Otherwise we can use BulletText() or TreeAdvanceToLabelPos()+Text().
			ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, game_objects_labels[i].c_str());
			if (ImGui::IsItemClicked())
				node_clicked = i;
		}*/
	}

	if (node_clicked != -1)
	{
		// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
		if (ImGui::GetIO().KeyCtrl)
			selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
			selection_mask = (1 << node_clicked);           // Click to single-select
	}
	if (child_clicked != -1)
	{
		// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
		if (ImGui::GetIO().KeyCtrl)
			selection_mask_child ^= (1 << child_clicked);          // CTRL+click to toggle
		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
			selection_mask_child = (1 << child_clicked);           // Click to single-select
	}

	ImGui::PopStyleVar();
	//ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
	ImGui::Indent(15.0f);
	ImGui::End();
}
