#include "ModuleEditor.h"
#include "Application.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
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
	bool show_test_window = true;
	bool show_another_window = false;
	LOG("pene");
	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindow());

	static float f = 0.0f;
	ImGui::Text("Hello, world!");
	/*ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	if (ImGui::Button("Test Window")) show_test_window ^= 1;
	if (ImGui::Button("Another Window")) show_another_window ^= 1;
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);*/

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