#ifndef PANELINTERFACE_H
#define PANELINTERFACE_H

#include "Panel.h"

#include <vector>

class PanelInterface : public Panel
{
public:
	PanelInterface();
	~PanelInterface();

	void Draw();

public:
	std::vector<std::string> game_objects_labels;
};

#endif