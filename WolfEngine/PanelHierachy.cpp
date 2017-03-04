#include "PanelHierachy.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleLevel.h"
#include "GameObject.h"

PanelHierachy::PanelHierachy() : Panel("Hierachy")
{
	active = new bool(true);
}

PanelHierachy::~PanelHierachy()
{}

void PanelHierachy::Draw()
{
	bool* b = new bool(true);

	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(App->window->GetScreenWidth() / 5, App->window->GetScreenHeight() - App->window->GetScreenHeight() / 3 - 20));

	ImGui::Begin("Hierachy", b, ImVec2(App->window->GetScreenWidth() / 5, App->window->GetScreenHeight() / 1.58f), -1.0f, ImGuiWindowFlags_ChildWindowAutoFitX | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_ChildWindowAutoFitY | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	//ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
	ImGui::Unindent(15.0f);

	static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
	static int selection_mask_child = (1 << 2);
	int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
	int child_clicked = -1;
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.
	for (int i = 0; i < game_objects_labels.size(); i++)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);
		if (game_objects_labels[i] == "Root")
		{
			// Node
			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, App->level->GetRoot()->childs[0]->name.c_str());
			if (ImGui::IsItemClicked())
				node_clicked = i;
			if (node_open)
			{
				for (int j = 0; j < App->level->GetRoot()->childs[0]->childs.size(); ++j)
				{
					node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask_child & (1 << j)) ? ImGuiTreeNodeFlags_Selected : 0);
					ImGui::TreeNodeEx((void*)(intptr_t)(j + i), node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, App->level->GetRoot()->childs[0]->childs[j]->name.c_str());
					if (ImGui::IsItemClicked())
					{
						child_clicked = j;
					}
				}
				ImGui::TreePop();
			}
		}
		else
		{
			// Leaf: The only reason we have a TreeNode at all is to allow selection of the leaf. Otherwise we can use BulletText() or TreeAdvanceToLabelPos()+Text().
			ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, game_objects_labels[i].c_str());
			if (ImGui::IsItemClicked())
				node_clicked = i;
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
