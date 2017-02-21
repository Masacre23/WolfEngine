#ifndef MODULEEDITOR_H
#define MODULEEDITOR_H

#include "Module.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include <vector>

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	void HandleInput(SDL_Event* event);
	void DrawEditor();
	
	void Draw(const char* title, bool* p_opened = NULL);
	void Console();
	void MenuBar();
	void Configuration();
	void AddLog(const char* fmt, ...);
public:
	ImGuiTextBuffer Buf;
	bool ScrollToBottom = false;
	bool show_test_window = false;
	bool* show_configuration;
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	int size_vector = 100;

	bool fullscreen;
	bool resizable;
	bool borderless;
	bool full_desktop;
};

#endif