#ifndef PANELHIERACHY_H
#define PANELHIERACHY_H

#include "Panel.h"

#include <vector>

class PanelHierachy : public Panel
{
public:
	PanelHierachy();
	~PanelHierachy();

	void Draw();

public:
	std::vector<std::string> game_objects_labels;
};

#endif