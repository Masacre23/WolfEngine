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
class RenderDebugDrawer;

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

private:
	void ResetProjection();

	bool ConstantConfig();
	bool GetGLError() const;

public:
	SDL_Renderer* renderer = nullptr;
	RenderDebugDrawer* debug_drawer = nullptr;
	bool draw_debug = true;

private:
	SDL_GLContext glcontext = NULL;
	float DEFAULT_SPEED = 1.0f;
	bool vsync = true;

	PrimitivePlane* base_plane = nullptr;
};

class RenderDebugDrawer
{
public:
	RenderDebugDrawer();
	~RenderDebugDrawer();

	void PreDebugDraw();
	void PostDebugDraw();

	void SetColor(const Color& color);
	void DrawAxis(const float4x4& transform = float4x4::identity);

	void DrawBoundingBox(const AABB& bbox, const Color& color, const float4x4& transform = float4x4::identity);
	void DrawBoundingBox(const OBB& bbox, const Color& color, const float4x4& transform = float4x4::identity);
	void DrawFrustum(const Frustum& frustum, const Color& color, const float4x4& transform = float4x4::identity);
	void DrawLine(const float3& from, const float3& to, const Color& color, const float4x4& transform = float4x4::identity);
	void DrawPoint(const float3& point, const Color& color, const float4x4& transform = float4x4::identity);

private:
	void DrawParallepiped(const float3* corners, const Color& color);
};

#endif // !MODULERENDER_H
