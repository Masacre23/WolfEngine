#ifndef PANELCONFIGURATION_H
#define PANELCONFIGURATION_H

#include "Panel.h"
#include <vector>

class PanelConfiguration : public Panel
{
public:
	PanelConfiguration();
	~PanelConfiguration();

	void Draw(ImGuiStyle* ref);

public:
	std::vector<float> fps_log;
	std::vector<float> ms_log;

	bool fullscreen;
	bool resizable;
	bool borderless;
	bool full_desktop;
};

#endif