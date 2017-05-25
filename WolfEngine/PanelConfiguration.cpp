#include "PanelConfiguration.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleTimeController.h"
#include "ModuleRender.h"
#include "ModuleLevel.h"
#include "ComponentCamera.h"
#include "SDL\include\SDL.h"
#include "Math.h"

PanelConfiguration::PanelConfiguration(bool active) : Panel("Configuration", active)
{
	for (int i = 0; i < size_vector; ++i)
	{
		fps_log.push_back(0);
		ms_log.push_back(0);
	}
}

PanelConfiguration::~PanelConfiguration()
{}

void PanelConfiguration::Draw()
{
	BROFILER_CATEGORY("PanelConfiguration-Draw", Profiler::Color::Azure);

	ImGui::Begin("Configuration", &active, ImGuiWindowFlags_ShowBorders);
	ImGui::Text("Options");

	if (ImGui::CollapsingHeader("Application"))
	{
		static char str0[128] = "Wolf Engine";
		static char str1[128] = "Sleepy Creepy";
		//str0 = App->window->TITLE;

		ImGui::InputText("App Name", str0, sizeof(str0));
		ImGui::InputText("Organization", str1, sizeof(str1));

		bool vsync = App->renderer->GetVsync();
		ImGui::Checkbox("Vsync", &vsync);
		App->renderer->SetVsync(vsync);

		ImGui::SameLine();

		int fps = App->time_controller->GetFpsCap();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.25f);
		ImGui::SliderInt("Max FPS", &fps, 0, 400);
		App->time_controller->SetFpsCap(fps);

		std::vector<float> aux_fps = fps_log;
		std::vector<float> aux_ms = ms_log;
		fps_log.clear();
		ms_log.clear();
		for (std::vector<float>::iterator it = aux_fps.begin() + 1; it != aux_fps.end(); ++it)
		{
			fps_log.push_back(*it);
		}

		for (std::vector<float>::iterator it = aux_ms.begin() + 1; it != aux_ms.end(); ++it)
		{
			ms_log.push_back(*it);
		}

		fps_log.push_back((float)App->time_controller->GetFramesLastSecond());

		ms_log.push_back((float)App->time_controller->GetLastFrameMs());

		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps_log.back());
		ImGui::PlotHistogram("##framerate", &fps_log.front(), fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

		sprintf_s(title, 25, "Milliseconds %0.1f", ms_log.back());
		ImGui::PlotHistogram("##milliseconds", &ms_log.front(), ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}

	if (ImGui::CollapsingHeader("Editor Camera"))
	{
		ComponentCamera* editor_camera = App->camera->editor_camera;

		ImGui::Checkbox("Frustum culling", &editor_camera->frustum_culling);

		if (ImGui::Button("Default camera configuration"))
			App->camera->SetupFrustum(editor_camera);

		float near_plane = editor_camera->frustum->NearPlaneDistance();
		if (ImGui::DragFloat("Near Plane", &near_plane, 0.1f, 0.1f, 1000.0f))
		{
			if (near_plane >= 0.1f && near_plane < editor_camera->frustum->FarPlaneDistance())
				editor_camera->SetPlaneDistances(near_plane, editor_camera->frustum->FarPlaneDistance());
		}

		float far_plane = editor_camera->frustum->FarPlaneDistance();
		if (ImGui::DragFloat("Far Plane", &far_plane, 1.0f, 10.0f, 10000.0f))
		{
			if (far_plane >= 10.0f && far_plane > editor_camera->frustum->NearPlaneDistance())
				editor_camera->SetPlaneDistances(editor_camera->frustum->NearPlaneDistance(), far_plane);
		}

		float vertical_fov = editor_camera->frustum->VerticalFov() * RAD_TO_DEG;
		if (ImGui::SliderFloat("Vertical FOV", &vertical_fov, 1.0f, 180.0f))
			editor_camera->SetFOV(vertical_fov);

		float aspect_ratio = editor_camera->frustum->AspectRatio();
		if (ImGui::DragFloat("Aspect Ratio", &aspect_ratio, 0.1f, 0.1f, 1000.0f))
			editor_camera->SetAspectRatio(aspect_ratio);
	}

	if (ImGui::CollapsingHeader("Debug"))
	{
		ImGui::Checkbox("Debug draw", &App->renderer->draw_debug);

		ImGui::Separator();

		ImGui::Checkbox("Base plane", &App->renderer->draw_base_plane);

		ImGui::Checkbox("Quadtree structure", &App->level->draw_quadtree_structure);
	}

	if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::Text("Icon: *default*");
		static float brightness = 0;
		ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f);
		static int width = 0;
		ImGui::SliderInt("Width", &width, 0, 1920);
		static int height = 0;
		ImGui::SliderInt("Height", &height, 0, 1080);
		ImGui::Text("Refresh rate: %d", App->time_controller->GetFramesLastSecond());

		if (ImGui::Checkbox("Fullscreen", &fullscreen))
			App->window->FULLSCREEN = fullscreen;
		ImGui::SameLine();
		if (ImGui::Checkbox("Resizable", &resizable))
			App->window->RESIZABLE = resizable;
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Restart to apply");
		if (ImGui::Checkbox("Borderless", &borderless))
			App->window->BORDERLESS = borderless;
		ImGui::SameLine();
		if (ImGui::Checkbox("Full Desktop", &full_desktop))
			App->window->FULLSCREEN_DESKTOP = full_desktop;

		ImGui::ShowStyleEditor(App->editor->ref);
	}
	if (ImGui::CollapsingHeader("Hardware"))
	{
		SDL_version version;
		SDL_GetVersion(&version);
		ImGui::Text("SDL Version: ");
		ImGui::SameLine();
		ImGui::TextColored(yellow, "%d.%d.%d", version.major, version.minor, version.patch);

		ImGui::Separator();
		ImGui::Text("CPUs: ");
		ImGui::SameLine();
		ImGui::TextColored(yellow, "%d", SDL_GetCPUCount());
		ImGui::Text("System RAM: ");
		ImGui::SameLine();
		ImGui::TextColored(yellow, "%dGB", SDL_GetSystemRAM() / 1000);
		ImGui::Text("Caps: ");
		if (SDL_HasRDTSC())
		{
			ImGui::SameLine();
			ImGui::TextColored(yellow, "RDTSC,");
		}
		if (SDL_HasMMX())
		{
			ImGui::SameLine();
			ImGui::TextColored(yellow, "MMX,");
		}
		if (SDL_HasSSE())
		{
			ImGui::SameLine();
			ImGui::TextColored(yellow, "SSE,");
		}
		if (SDL_HasSSE2())
		{
			ImGui::SameLine();
			ImGui::TextColored(yellow, "SSE2,");
		}
		if (SDL_HasSSE3())
		{
			ImGui::SameLine();
			ImGui::TextColored(yellow, "SSE3,");
		}
		if (SDL_HasSSE41())
		{
			ImGui::SameLine();
			ImGui::TextColored(yellow, "SSE41,");
		}
		if (SDL_HasSSE42())
		{
			ImGui::SameLine();
			ImGui::TextColored(yellow, "SSE42,");
		}
		if (SDL_HasAVX())
		{
			ImGui::SameLine();
			ImGui::TextColored(yellow, "AVX,");
		}

		ImGui::Separator();
		ImGui::Text("GPU: ");
		ImGui::SameLine();
	}
	ImGui::End();
}