#include "PanelInterface.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleLevel.h"
#include "GameObject.h"
#include "PanelHierachy.h"

PanelInterface::PanelInterface() : Panel("Interface")
{
	active = new bool(true);
	hierachy = new PanelHierachy();
}

PanelInterface::~PanelInterface()
{
}

void PanelInterface::Draw()
{
	hierachy->Draw(game_objects);
	
	game_objects[0]->components[0]->OnEditor();

	bool* b = new bool(true);
	//static int selection_mask = (1 << 2);
	//int node_clicked = -1;

	ImGui::SetNextWindowPos(ImVec2(0, App->window->GetScreenHeight() - App->window->GetScreenHeight() / 3));
	ImGui::SetNextWindowSize(ImVec2(App->window->GetScreenWidth(), App->window->GetScreenHeight() / 3));
	ImGui::Begin("Project", b, ImVec2(App->window->GetScreenWidth(), App->window->GetScreenHeight() / 3), -1.0f, ImGuiWindowFlags_ChildWindowAutoFitX | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_ChildWindowAutoFitY | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::End();
}
