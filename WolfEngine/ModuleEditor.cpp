#include "ModuleEditor.h"
#include "Application.h"

#include <stdio.h>
#include "Glew/include/GL/glew.h"   
#include "SDL\include\SDL.h"

#include "ModuleWindow.h"

ModuleEditor::ModuleEditor() : Module("ModuleEditor", true)
{
}


ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
	ImGui_ImplSdlGL3_Init(App->window->GetWindow());
	for (int i = 0; i < size_vector; ++i)
	{
		fps_log.push_back(0);
		ms_log.push_back(0);
	}
	show_configuration = new bool(false);
	return true;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindow());

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{
	//bool show_another_window = false;

	//Console();

	//MenuBar();
	
	if(*show_configuration)
		Configuration();

	/*if(show_test_window)
		ImGui::ShowTestWindow();*/

	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

void ModuleEditor::HandleInput(SDL_Event* event)
{
	ImGui_ImplSdlGL3_ProcessEvent(event);
}

void ModuleEditor::DrawEditor()
{
	ImGui::Render();
}

void ModuleEditor::Draw(const char* title, bool* p_opened)
{
	ImGui::Begin(title, p_opened);
	ImGui::TextUnformatted(Buf.begin());
	if (ScrollToBottom)
		ImGui::SetScrollHere(1.0f);
	ScrollToBottom = false;
	ImGui::End();
}

void ModuleEditor::AddLog(const char* fmt, ...) IM_PRINTFARGS(2)
{
	va_list args;
	va_start(args, fmt);
	Buf.appendv(fmt, args);
	va_end(args);
	ScrollToBottom = true;
}

void ModuleEditor::Console()
{
	Draw("Console");
}

void ModuleEditor::MenuBar()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Configuration"))
	{
		if (ImGui::MenuItem("Options"))
			*show_configuration = true;
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Gui Demo"))
			show_test_window = !show_test_window;

		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}

void ModuleEditor::Configuration()
{
	ImGui::Begin("Configuration", show_configuration);
	ImGui::Text("Options");
	//if (ImGui::TreeNode("Options"))
	//{
		if (ImGui::CollapsingHeader("Application"))
		{
			static char str0[128] = "Wolf Engine";
			static char str1[128] = "Sleepy Creepy";
			ImGui::InputText("App Name", str0, sizeof(str0));
			ImGui::InputText("Organization", str1, sizeof(str1));
			static int i1 = 0;
			ImGui::SliderInt("Max FPS", &i1, 0, 120);

			
			//static float time = App->prev_time;

			//if (time * 10.0 < App->prev_time * 10.0)
			//{
				std::vector<float> aux_fps = fps_log;
				std::vector<float> aux_ms = ms_log;
				fps_log.clear();
				ms_log.clear();
				for (std::vector<float>::iterator it = aux_fps.begin()+1; it != aux_fps.end(); ++it)
				{
					fps_log.push_back(*it);
				}
				
				for (std::vector<float>::iterator it = aux_ms.begin()+1; it != aux_ms.end(); ++it)
				{
					ms_log.push_back(*it);
				}
			//	time = App->prev_time;

				fps_log.push_back(App->frames_last_sec);

				ms_log.push_back(App->last_frame_ms);
			//}			

			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", fps_log.back());
			ImGui::PlotHistogram("##framerate", &fps_log.front(), fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

			sprintf_s(title, 25, "Milliseconds %0.1f", ms_log.back());
			ImGui::PlotHistogram("##milliseconds", &ms_log.front(), ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));

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
			ImGui::Text("Refresh rate: %d", App->frames_last_sec);

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
		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
			SDL_version version;
			ImVec4 yellow = ImVec4(255, 255, 0, 255);
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
			ImGui::TextColored(yellow, "%dGB", SDL_GetSystemRAM()/1000);
			ImGui::Text("Caps: ");
			/*if (SDL_HasRDTSC())
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
			}*/
			ImGui::Separator();

		}
		//ImGui::TreePop();
	//}
	ImGui::End();
}