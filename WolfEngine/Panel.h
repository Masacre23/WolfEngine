#ifndef PANEL_H
#define PANEL_H

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include <string>

class Panel
{
public:
	Panel(const char* name);
	~Panel();

	//virtual void Draw() = 0;
	void Draw() { return; };

public:
	bool* active = new bool(true);
	int w, h, x, y;

	ImVec4 yellow = ImVec4(255, 255, 0, 255);
	std::string name;
};

#endif