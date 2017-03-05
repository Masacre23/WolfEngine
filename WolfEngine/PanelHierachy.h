#ifndef PANELHIERACHY_H
#define PANELHIERACHY_H

#include "Panel.h"
#include <vector>

class GameObject;

class PanelHierachy : public Panel
{
public:
	PanelHierachy();
	~PanelHierachy();

	void Draw(std::vector<GameObject*> game_objects);
};

#endif