#ifndef MODULERENDER_H
#define MODULERENDER_H

#include "Module.h"
#include "SDL/include/SDL_rect.h"
#include "Point.h"
#include "SDL/include/SDL_video.h"

#define MODULE_RENDER "ModuleRender"
#define RENDER_SECTION "Config.Modules.Render"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:
	bool ConstantConfig();
	bool GetGLError() const;
	void DebugCamera(float dt);

public:
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera = { 0,0,0,0 };

private:
	SDL_GLContext glcontext = NULL;
	int SCREENSIZE = 0;
	int SCREENWIDTH = 0;
	int SCREENHEIGHT = 0;
	float DEFAULT_SPEED = 1.0f;
	bool VSYNC = true;
};

#endif // !MODULERENDER_H
