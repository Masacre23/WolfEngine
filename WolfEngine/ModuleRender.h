#ifndef MODULERENDER_H
#define MODULERENDER_H

#include "Module.h"
#include "SDL/include/SDL_video.h"
#include "MathGeoLib/src/Math/float3.h"
#include "MathGeoLib/src/Geometry/Frustum.h"

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

	void DrawCube(unsigned int texture, float3 transform = { 0, 0, 0 }, float3 scale = { 1, 1, 1 }, float angle = 0, float3 rotation = { 0, 0, 0 });
	void DrawBoundingBox(const AABB& bbox, const Color& color);
	void DrawFrustum(const Frustum& frustum, const Color& color);
	void DrawAxis();

private:
	void ResetProjection();

	void LoadCubeGeometry();

	void DrawBasePlane(const Color& color);
	void DrawParallepiped(const float3* corners, const Color& color);

	bool SetVsync(bool vsync);

	bool ConstantConfig();
	bool GetGLError() const;

public:
	SDL_Renderer* renderer = nullptr;

private:
	SDL_GLContext glcontext = NULL;
	float DEFAULT_SPEED = 1.0f;
	bool VSYNC = true;

	unsigned int id_vertices;
	unsigned int id_texture;
	unsigned int id_indices_triangles;
	unsigned int id_normals;

	unsigned int id_texture_check;

	unsigned int num_indices_triangles;
	float angle = 0.0f;
};

#endif // !MODULERENDER_H
