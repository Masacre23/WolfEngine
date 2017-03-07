#ifndef PANELCONFIGURATION_H
#define PANELCONFIGURATION_H

#include "Panel.h"
#include <vector>

class PanelConfiguration : public Panel
{
public:
	PanelConfiguration(bool active = false);
	~PanelConfiguration();

	void Draw();

public:
	std::vector<float> fps_log;
	std::vector<float> ms_log;

	bool fullscreen;
	bool resizable;
	bool borderless;
	bool full_desktop;

	int size_vector = 100;
};

#endif