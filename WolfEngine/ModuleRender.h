#ifndef MODULERENDER_H
#define MODULERENDER_H

#include "Module.h"
#include "SDL/include/SDL_video.h"
#include "MathGeoLib/src/Math/float3.h"

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
	void LoadCubeGeometry();

	void DrawCube(float3 transform = { 0, 0, 0 }, float3 scale = { 1, 1, 1 }, float angle = 0, float3 rotation = { 0, 0, 0 });
	void DrawBasePlane();
	void DrawAxis();

	bool ConstantConfig();
	bool GetGLError() const;

public:
	SDL_Renderer* renderer = nullptr;

private:
	SDL_GLContext glcontext = NULL;
	int SCREENSIZE = 0;
	int SCREENWIDTH = 0;
	int SCREENHEIGHT = 0;
	float DEFAULT_SPEED = 1.0f;
	bool VSYNC = true;

	unsigned int id_vertices;
	unsigned int num_vertices; 
	unsigned int id_colors_edges;
	unsigned int id_colors_faces;
	unsigned int id_colors_diagonals;
	unsigned int num_colors;

	unsigned int id_indices_triangles;
	unsigned int num_indices_triangles;
	unsigned int id_indices_edges;
	unsigned int num_indices_edges;
	unsigned int id_indices_diagonals;
	unsigned int num_indices_diagonals;
	float angle = 0.0f;
};

#endif // !MODULERENDER_H
