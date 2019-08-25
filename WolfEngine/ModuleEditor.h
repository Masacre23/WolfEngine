#ifndef MODULEEDITOR_H
#define MODULEEDITOR_H

#define MODULE_EDITOR "ModuleEditor"

#include "Module.h"
#include "Interface.h"
#include <vector>
#include <string>

class Panel;
class PanelMenuBar;
class PanelInterface;
class PanelConsole;
class PanelGameTime;

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
	void Draw() const;
	
	void AddLog(const char* fmt);
public:
	//bool show_test_window = false;
	bool* show_configuration;
	bool* show_about;
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	int size_vector = 100;

	ImVec4 yellow = ImVec4(255, 255, 0, 255);
	ImGuiStyle* ref;

	PanelConsole* console = nullptr;

	PanelMenuBar* menu = nullptr;
	PanelInterface* interfaces = nullptr;
	PanelGameTime* game_time = nullptr;

private:
	std::vector<Panel*> panels;
};

#endif