#ifndef MODULEWINDOW_H
#define MODULEWINDOW_H

#include "Module.h"
#include "Point.h"

#define MODULE_WINDOW "ModuleWindow"
#define WINDOW_SECTION "Config.Modules.Window"

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{
public:
	ModuleWindow();
	~ModuleWindow();

	bool Init();
	bool CleanUp();

	int GetScreenWidth() const { return SCREENWIDTH; }
	int GetScreenHeight() const { return SCREENHEIGHT; }
	int GetScreenSize() const { return SCREENSIZE; }

private:
	bool ConstantConfig();

public:
	SDL_Window* window = nullptr;
	SDL_Surface* screen_surface = nullptr;

private:
	const char* TITLE = "";
	int SCREENWIDTH = 0;
	int SCREENHEIGHT = 0;
	int SCREENSIZE = 0;

	bool FULLSCREEN = false;
	bool BORDERLESS = false;
	bool RESIZABLE = false;
	bool FULLSCREEN_DESKTOP = false;
};

#endif // !MODULEWINDOW_H
