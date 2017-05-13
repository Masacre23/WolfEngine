#include "PanelGameTime.h"
#include "Application.h"
#include "ModuleTimeController.h"

PanelGameTime::PanelGameTime(bool active) : Panel("Game Time", active)
{
}

PanelGameTime::~PanelGameTime()
{
}

void PanelGameTime::Draw()
{
	ImGui::Begin("", new bool(true), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_AlwaysAutoResize);
	if (App->time_controller->IsStopped())
	{
		if (ImGui::Button("Play"))
		{
			App->time_controller->Play();
		}
	}
	else
	{
		if (ImGui::Button("Stop"))
		{
			App->time_controller->Stop();
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			App->time_controller->Pause();
		}
		ImGui::SameLine();
		if (ImGui::Button("Tick"))
		{
			App->time_controller->Tick();
		}
	}
	
	ImGui::SliderFloat("Speed", &App->time_controller->time_scale, 0.1, 10);
	
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Stats"))
	{
		ImGui::Text("Game delta time: %f", App->time_controller->GetDeltaTime());

		ImGui::Text("Game time: %f", App->time_controller->GetGameTime());

		ImGui::Text("Game frame count: %d", App->time_controller->GetFrameCount());
	}

	ImGui::End();
}