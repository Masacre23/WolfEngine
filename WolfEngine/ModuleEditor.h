#ifndef MODULEEDITOR_H
#define MODULEEDITOR_H

#include "Module.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void Draw(const char* title, bool* p_opened = NULL);
	void AddLog(const char* fmt, ...);
public:
	ImGuiTextBuffer Buf;
	bool ScrollToBottom = false;
};

#endif