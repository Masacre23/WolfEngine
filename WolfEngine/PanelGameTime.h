#ifndef PANELGAMETIME_H
#define PANELGAMETIME_H

#include "Panel.h"

class PanelGameTime : public Panel
{
public:
	PanelGameTime(bool active = true);
	~PanelGameTime();

	void Draw();

private:

};

#endif // !PANELGAMETIME_H