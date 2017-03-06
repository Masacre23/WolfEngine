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

	int Draw(std::vector<GameObject*> game_objects);
	void DrawChilds(GameObject* game_object, int &i, bool node_open);

public:
	int selection_mask = (1 << 2);
	int node_clicked = -1;
};

#endif