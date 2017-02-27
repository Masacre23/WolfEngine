#ifndef PANEL_H
#define PANEL_H

#include <string>

class Panel
{
public:
	Panel(const char* name);
	~Panel();

	virtual void Draw() = 0;

public:
	bool active = true;
	int w, h, x, y;

	std::string name;
};

#endif