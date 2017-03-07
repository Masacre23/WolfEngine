#ifndef PANELCONSOLE_H
#define PANELCONSOLE_H

#include "Panel.h"

class PanelConsole : public Panel
{
public:
	PanelConsole(bool active = false);
	~PanelConsole();

	void Draw();

	void Clear();
	void AddLog(const char* fmt);

private:
	ImGuiTextBuffer Buf;
	bool ScrollToBottom = false;
};

#endif