#ifndef PANELINTERFACE_H
#define PANELINTERFACE_H

#include "Panel.h"
#include <vector>

class PanelHierachy;
class GameObject;

class PanelInterface : public Panel
{
public:
	PanelInterface();
	~PanelInterface();

	void Draw();

public:
	PanelHierachy* hierachy;
	std::vector<GameObject*> game_objects;
};

#endif