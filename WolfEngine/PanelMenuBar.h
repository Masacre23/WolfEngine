#ifndef PANELMENUBAR_H
#define PANELMENUBAR_H

#include "Panel.h"

class PanelAbout;
class PanelConfiguration;
struct ImGuiStyle;

class PanelMenuBar : public Panel
{
public:
	PanelMenuBar(bool active = true);
	~PanelMenuBar();

	void Draw();

public:
	PanelAbout* about;
	PanelConfiguration* config;
	bool show_test_window = false;
};

#endif