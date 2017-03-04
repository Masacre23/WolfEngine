#ifndef PANELINTERFACE_H
#define PANELINTERFACE_H

#include "Panel.h"

class PanelHierachy;

class PanelInterface : public Panel
{
public:
	PanelInterface();
	~PanelInterface();

	void Draw();

public:
	PanelHierachy* hierachy;
};

#endif