#ifndef MODULERENDER_H
#define MODULERENDER_H

#include "Module.h"
#include "SDL/include/SDL_video.h"
#include "Math.h"

#define MODULE_RENDER "ModuleRender"
#define RENDER_SECTION "Config.Modules.Render"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class Color;

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

	void WindowResize(int width, int height);
	
	void DrawColor(const Color& color);

	void PreDebugDraw();
	void PostDebugDraw();
	void DrawBoundingBox(const AABB& bbox, const Color& color);
	void DrawBoundingBox(const OBB& bbox, const Color & color);
	void DrawFrustum(const Frustum& frustum, const Color& color);
	void DrawAxis();

	bool GetVsync() const { return vsync; }
	bool SetVsync(bool active);

private:
	void ResetProjection();

	void DrawBasePlane(const Color& color);
	void DrawParallepiped(const float3* corners, const Color& color);

	bool ConstantConfig();
	bool GetGLError() const;

	void LoadPrimitivesGeometry();

public:
	SDL_Renderer* renderer = nullptr;
	bool debug_draw = true;

private:
	SDL_GLContext glcontext = NULL;
	float DEFAULT_SPEED = 1.0f;
	bool vsync = true;

	unsigned int id_vertices;
	unsigned int id_texture;
	unsigned int id_indices_triangles;
	unsigned int id_normals;

	unsigned int id_texture_check;

	unsigned int num_indices_triangles;
	float angle = 0.0f;
};

#endif // !MODULERENDER_H
