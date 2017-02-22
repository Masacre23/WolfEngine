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
	bool CleanUp();

	void SetFPStoWindow(int total_frames, float total_seconds, Uint32 update_ms, int current_fps, float dt);

	SDL_Window* GetWindow() const { return window; }
	int GetScreenWidth() const { return screen_width; }
	int GetScreenHeight() const { return screen_height; }

	void WindowResize(int width, int height);

private:
	bool ConstantConfig();

public:
	bool FULLSCREEN = false;
	bool BORDERLESS = false;
	bool RESIZABLE = false;
	bool FULLSCREEN_DESKTOP = false;
	SDL_Window* window = nullptr;
	const char* TITLE = "";

private:
	SDL_Surface* screen_surface = nullptr;

	char* title_fps = nullptr;
	int screen_width = 800;
	int screen_height = 600;

	int TITLE_INFO_LENGTH = 200;
};

#endif // !MODULEWINDOW_H
