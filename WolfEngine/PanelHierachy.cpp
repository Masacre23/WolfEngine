#include "PanelHierachy.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleLevel.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Component.h"

PanelHierachy::PanelHierachy(bool active) : Panel("Hierachy", active)
{
}

PanelHierachy::~PanelHierachy()
{}

GameObject* PanelHierachy::DrawInterfaceHierachy()
{
	bool b = true;
	static GameObject* ret = nullptr;
	id = 0;

	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(App->window->GetScreenWidth() / 5, App->window->GetScreenHeight() - App->window->GetScreenHeight() / 3 - 20));

	ImGui::Begin("Hierachy", &b, ImVec2(App->window->GetScreenWidth() / 5, App->window->GetScreenHeight() / 1.58f), -1.0f, ImGuiWindowFlags_ChildWindowAutoFitX | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_ChildWindowAutoFitY | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::Unindent(15.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.
	for (int i = 0; i < App->level->GetRoot()->childs.size(); ++i)
	{
		//if(node_clicked == -1)
		//	++id;
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);
		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, App->level->GetRoot()->childs[i]->name.c_str());
		if (ImGui::IsItemClicked())
		{
			node_clicked = i;
			ret = App->level->GetRoot()->childs[i];
		}
		if (node_open)
		{
			GameObject* go = DrawChilds(App->level->GetRoot()->childs[i], i, node_open);
			if (go != nullptr)
				ret = go;
		}
	}

	if (node_clicked != -1 && id != -1)
	{
		// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
		if (ImGui::GetIO().KeyCtrl)
			selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
			selection_mask = (1 << node_clicked);           // Click to single-select
	}

	ImGui::PopStyleVar();
	ImGui::Indent(15.0f);
	ImGui::End();

	return ret;
}

GameObject* PanelHierachy::DrawChilds(GameObject* game_object, int &i, bool node_open)
{
	GameObject* ret = nullptr;
	if (node_open && game_object->childs.size() != 0)
	{
		for (int j = i + 1; j < game_object->childs.size() + i + 1; ++j)
		{
			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << j)) ? ImGuiTreeNodeFlags_Selected : 0);
			if (game_object->childs[j - i - 1]->childs.size() != 0)
			{
				bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)j, node_flags, game_object->childs[j - i - 1]->name.c_str());
				if (ImGui::IsItemClicked())
				{
					node_clicked = j;
					ret = game_object->childs[j - i - 1];
				}

				int n = j;

				GameObject* go = DrawChilds(game_object->childs[j - i - 1], n, node_open);

				if (go != nullptr)
					ret = go;
			}
			else
			{
				ImGui::TreeNodeEx((void*)(intptr_t)j, node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, game_object->childs[j - i - 1]->name.c_str());
				if (ImGui::IsItemClicked())
				{
					node_clicked = j;
					ret = game_object->childs[j - i - 1];
				}

				int n = j ;

				GameObject* go = DrawChilds(game_object->childs[j - i - 1], n, false);

				if (go != nullptr)
					ret = go;
			}
			
		}
		ImGui::TreePop();
	}
	return ret;
}
