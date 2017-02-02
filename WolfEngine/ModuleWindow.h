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
	bool Start();
	update_status PreUpdate(float dt);
	bool CleanUp();

	void SetFPStoWindow(int total_frames, float total_seconds, Uint32 update_ms, int current_fps, float dt);

	SDL_Window* GetWindow() const { return window; }
	int GetScreenWidth() const { return SCREENWIDTH; }
	int GetScreenHeight() const { return SCREENHEIGHT; }
	int GetScreenSize() const { return SCREENSIZE; }

private:
	bool ConstantConfig();

private:
	SDL_Window* window = nullptr;
	SDL_Surface* screen_surface = nullptr;

	char* title_fps = nullptr;
	int width = 0;
	int height = 0;

	const char* TITLE = "";
	int SCREENWIDTH = 0;
	int SCREENHEIGHT = 0;
	int SCREENSIZE = 0;

	bool FULLSCREEN = false;
	bool BORDERLESS = false;
	bool RESIZABLE = false;
	bool FULLSCREEN_DESKTOP = false;
	int TITLE_INFO_LENGTH = 200;
};

#endif // !MODULEWINDOW_H
