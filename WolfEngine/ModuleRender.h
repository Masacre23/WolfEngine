#ifndef MODULERENDER_H
#define MODULERENDER_H

#include "Module.h"
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
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:
	bool ConstantConfig();
	bool GetGLError() const;
	void DebugCubeVertices();
	void DebugCube();

public:
	SDL_Renderer* renderer = nullptr;

private:
	SDL_GLContext glcontext = NULL;
	int SCREENSIZE = 0;
	int SCREENWIDTH = 0;
	int SCREENHEIGHT = 0;
	float DEFAULT_SPEED = 1.0f;
	bool VSYNC = true;

	unsigned int debug_id_vertices;
	unsigned int debug_num_vertices; 
	unsigned int debug_id_indices;
	unsigned int debug_num_indices;
	float angle = 0.0f;
};

#endif // !MODULERENDER_H
