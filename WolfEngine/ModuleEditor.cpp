#include "ModuleEditor.h"
#include "Application.h"


#include <stdio.h>
#include "Glew/include/GL/glew.h"   
#include "SDL\include\SDL.h"

#include "ModuleWindow.h"
//#include "ModuleInput.h"

ModuleEditor::ModuleEditor() : Module("ModuleEditor", true)
{
}


ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->GetWindow());
	return true;
}

bool ModuleEditor::Start()
{
	return true;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{
	
	//bool show_another_window = false;
	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindow());

	Draw("Console");

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
		//if(ImGui::MenuItem("Documentation"))
		

		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();

	if(show_test_window)
		ImGui::ShowTestWindow();

	ImGui::Render();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
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