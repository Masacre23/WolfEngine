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
class PrimitivePlane;

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
	
	bool GetVsync() const { return vsync; }
	bool SetVsync(bool active);

	void DrawColor(const Color& color);
	void PreDebugDraw();
	void PostDebugDraw();
	void DrawBoundingBox(const AABB& bbox, const Color& color);
	void DrawBoundingBox(const OBB& bbox, const Color& color);
	void DrawFrustum(const Frustum& frustum, const Color& color);
	void DrawAxis();

private:
	void ResetProjection();

	void DrawParallepiped(const float3* corners, const Color& color);

	bool ConstantConfig();
	bool GetGLError() const;

public:
	SDL_Renderer* renderer = nullptr;
	bool debug_draw = true;

private:
	SDL_GLContext glcontext = NULL;
	float DEFAULT_SPEED = 1.0f;
	bool vsync = true;

	PrimitivePlane* base_plane = nullptr;
};

#endif // !MODULERENDER_H
