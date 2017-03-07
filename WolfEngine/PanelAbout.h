#ifndef PANELABOUT_H
#define PANELABOUT_H

#include "Panel.h"

class PanelAbout : public Panel
{
public:
	PanelAbout(bool active = false);
	~PanelAbout();

	void Draw();
};

#endif