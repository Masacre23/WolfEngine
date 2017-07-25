#include "PanelInterface.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleLevel.h"
#include "GameObject.h"
#include "PanelHierachy.h"
#include "ComponentTransform.h"

PanelInterface::PanelInterface(bool active) : Panel("Interface", active)
{
	hierachy = new PanelHierachy();
}

PanelInterface::~PanelInterface()
{
	RELEASE(hierachy);
}

void PanelInterface::Draw()
{
	BROFILER_CATEGORY("PanelInterface-Draw", Profiler::Color::Azure);

	bool b = true;
	ImGui::SetNextWindowPos(ImVec2(App->window->GetScreenWidth() * 4 / 5, 20));
	ImGui::SetNextWindowSize(ImVec2(App->window->GetScreenWidth() / 5, App->window->GetScreenHeight() - App->window->GetScreenHeight() / 3 - 20));
	ImGui::Begin("Inspector", &b, ImVec2(App->window->GetScreenWidth() / 5, App->window->GetScreenHeight() / 1.58f), -1.0f, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders);
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents

	GameObject* current_selected = hierachy->DrawInterfaceHierachy();
	GameObject* saved_selected = App->level->GetSelectedGameObject();
	if (current_selected != saved_selected)
	{
		if (saved_selected)
			saved_selected->selected = false;
		App->level->SetSelectedGameObject(current_selected);
	}

	if (current_selected)
	{
		current_selected->selected = true;
		current_selected->OnEditor();
	}

	ImGui::PopStyleVar();
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, App->window->GetScreenHeight() - App->window->GetScreenHeight() / 3));
	ImGui::SetNextWindowSize(ImVec2(App->window->GetScreenWidth(), App->window->GetScreenHeight() / 3));
	ImGui::Begin("Project", &b, ImVec2(App->window->GetScreenWidth(), App->window->GetScreenHeight() / 3), -1.0f, ImGuiWindowFlags_ChildWindowAutoFitX | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_ChildWindowAutoFitY | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders);
	ImGui::End();
}
