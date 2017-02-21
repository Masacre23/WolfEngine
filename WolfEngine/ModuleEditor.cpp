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

	Console();

	MenuBar();
	
	FPSgraph();

	if(show_test_window)
		ImGui::ShowTestWindow();

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
		ImGui::Text("Options");
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

void ModuleEditor::FPSgraph()
{
	ImGui::Begin("Configuration");
	if (ImGui::TreeNode("Options"))
	{
		if (ImGui::CollapsingHeader("Application"))
		{
			static char str0[128] = "Wolf Engine";
			static char str1[128] = "Sleepy Creepy";
			ImGui::InputText("App Name", str0, sizeof(str0));
			ImGui::InputText("Organization", str1, sizeof(str1));
			static int i1 = 0;
			ImGui::SliderInt("Max FPS", &i1, 0, 120);
		}
		ImGui::TreePop();
	}
	ImGui::End();
}