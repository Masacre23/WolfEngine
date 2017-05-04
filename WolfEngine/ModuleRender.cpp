#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleTextures.h"
#include "ModuleEditor.h"
#include "ModuleLevel.h"
#include "SDL/include/SDL.h"
#include "OpenGL.h"
#include "Math.h"
#include "JsonHandler.h"
#include "Color.h"
#include "Primitive.h"

#pragma comment( lib, "Glew/libx86/glew32.lib" )
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

ModuleRender::ModuleRender() : Module(MODULE_RENDER)
{
}

ModuleRender::~ModuleRender()
{}

bool ModuleRender::Init()
{
	APPLOG("Creating 3D Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if (ConstantConfig() == false)
	{
		APPLOG("Problem retrieving value from configuration file");
		ret = false;
	}

	//Creating OpenGL Context for SDL
	glcontext = SDL_GL_CreateContext(App->window->GetWindow());
	if (glcontext == NULL)
	{
		APPLOG("GL Context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	//Initialize Glew
	GLenum err = glewInit();
	if (err != GL_NO_ERROR)
	{
		APPLOG("Error during Glew library init: %s\n", glewGetErrorString(err));
		ret = false;
	}
	else
		APPLOG("Using Glew %s", glewGetString(GLEW_VERSION));

	if (ret == true)
	{
		//Get hardware and drivers capabilities
		APPLOG("Vendor: %s", glGetString(GL_VENDOR));
		APPLOG("Renderer: %s", glGetString(GL_RENDERER));
		APPLOG("OpenGL version supported %s", glGetString(GL_VERSION));
		APPLOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		ret = GetGLError();
		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		ret = ret && GetGLError();
		//Other options
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		ret = ret && GetGLError();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		ret = ret && GetGLError();

		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);

		GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

		GLfloat light_position[] = { 4.0f, 8.0f, 4.0f, 1.0f };
		GLfloat ambient[] = { 1.0f, 1.0, 1.0f, 1.0f };

		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

		ret = ret && GetGLError();

		SetVsync(VSYNC);

		ret = ret && GetGLError();
	}

	return ret;
}

bool ModuleRender::Start()
{
	LoadPrimitivesGeometry();

	return true;
}

update_status ModuleRender::PreUpdate(float dt)
{
	BROFILER_CATEGORY("ModuleRender_PreUpdate", Profiler::Color::Blue);

	glViewport(0, 0, App->window->GetScreenWidth(), App->window->GetScreenHeight());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	ResetProjection();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	return UPDATE_CONTINUE;
}

update_status ModuleRender::Update(float dt)
{
	BROFILER_CATEGORY("ModuleRender_Update", Profiler::Color::Red);

	DrawBasePlane(Colors::White);
	DrawAxis();

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate(float dt)
{
	BROFILER_CATEGORY("ModuleRender_PostUpdate", Profiler::Color::Green);

	//App->level->Draw();
	App->level->DrawDebug();
	App->editor->Draw();
	SDL_GL_SwapWindow(App->window->GetWindow());

	return UPDATE_CONTINUE;
}

bool ModuleRender::CleanUp()
{
	APPLOG("Destroying 3d renderer");

	if (glcontext != NULL)
		SDL_GL_DeleteContext(glcontext);

	return true;
}

void ModuleRender::WindowResize(int width, int height)
{
	App->camera->SetAspectRatio((float)width / (float)height);
}

void ModuleRender::ResetProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(App->camera->GetProjectionMatrix());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRender::DrawColor(const Color& color)
{
	glColor3f(color.r, color.g, color.b);
}

void ModuleRender::DrawBoundingBox(const AABB& bbox, const Color& color)
{
	float3 corners[8];
	bbox.GetCornerPoints(corners);

	DrawParallepiped(corners, color);
}

void ModuleRender::DrawBoundingBox(const OBB& bbox, const Color& color)
{
	float3 corners[8];
	bbox.GetCornerPoints(corners);

	DrawParallepiped(corners, color);
}

void ModuleRender::DrawFrustum(const Frustum& frustum, const Color& color)
{
	float3 corners[8];
	frustum.GetCornerPoints(corners);

	DrawParallepiped(corners, color);
}

void ModuleRender::DrawAxis()
{
	float axis_length = 1.5f;

	glDepthRange(0.0, 0.01);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glLineWidth(2.0f);

	DrawColor(Colors::Red);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(axis_length, 0.0, 0.0);
	glEnd();

	DrawColor(Colors::Green);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, axis_length, 0.0);
	glEnd();

	DrawColor(Colors::Blue);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, axis_length);
	glEnd();

	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	glDepthRange(0.01, 1.0);

}

void ModuleRender::DrawBasePlane(const Color& color)
{
	float distance_between_lines = 1.0f;
	float min_distance = -100.f;
	float max_distance = 100.f;
	int num_lines = ((int)((max_distance - min_distance) / distance_between_lines)) + 1;

	//glDepthRange(0.1, 0.9);
	
	glLineWidth(2.0f);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	DrawColor(color);
	for (int i = 0; i < num_lines; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(min_distance + i*distance_between_lines, 0.0, min_distance);
		glVertex3f(min_distance + i*distance_between_lines, 0.0, max_distance);
		glEnd();
	}
	for (int i = 0; i < num_lines; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(min_distance, 0.0, min_distance + i*distance_between_lines);
		glVertex3f(max_distance, 0.0, min_distance + i*distance_between_lines);
		glEnd();
	}
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	
}

void ModuleRender::DrawParallepiped(const float3* corners, const Color& color)
{
	glLineWidth(1.0f);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_LINES);
	
	DrawColor(color);
	glVertex3f(corners[0].x, corners[0].y, corners[0].z);
	glVertex3f(corners[1].x, corners[1].y, corners[1].z);

	glVertex3f(corners[0].x, corners[0].y, corners[0].z);
	glVertex3f(corners[2].x, corners[2].y, corners[2].z);

	glVertex3f(corners[0].x, corners[0].y, corners[0].z);
	glVertex3f(corners[4].x, corners[4].y, corners[4].z);

	glVertex3f(corners[5].x, corners[5].y, corners[5].z);
	glVertex3f(corners[1].x, corners[1].y, corners[1].z);

	glVertex3f(corners[5].x, corners[5].y, corners[5].z);
	glVertex3f(corners[4].x, corners[4].y, corners[4].z);

	glVertex3f(corners[5].x, corners[5].y, corners[5].z);
	glVertex3f(corners[7].x, corners[7].y, corners[7].z);

	glVertex3f(corners[3].x, corners[3].y, corners[3].z);
	glVertex3f(corners[7].x, corners[7].y, corners[7].z);

	glVertex3f(corners[3].x, corners[3].y, corners[3].z);
	glVertex3f(corners[1].x, corners[1].y, corners[1].z);

	glVertex3f(corners[3].x, corners[3].y, corners[3].z);
	glVertex3f(corners[2].x, corners[2].y, corners[2].z);

	glVertex3f(corners[6].x, corners[6].y, corners[6].z);
	glVertex3f(corners[7].x, corners[7].y, corners[7].z);

	glVertex3f(corners[6].x, corners[6].y, corners[6].z);
	glVertex3f(corners[4].x, corners[4].y, corners[4].z);

	glVertex3f(corners[6].x, corners[6].y, corners[6].z);
	glVertex3f(corners[2].x, corners[2].y, corners[2].z);

	DrawColor(Colors::Black);
	glEnd();
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}

bool ModuleRender::SetVsync(bool vsync)
{
	bool ret = true;
	if (SDL_GL_SetSwapInterval(vsync ? 1 : 0))
		APPLOG("Error during Vsync setting: %s", SDL_GetError());
	return ret;
}

bool ModuleRender::ConstantConfig()
{
	bool ret = true;

	if (App->parser->LoadObject(RENDER_SECTION) == true)
	{
		VSYNC = App->parser->GetBoolMandatory("Vsync");
		DEFAULT_SPEED = App->parser->GetFloat("DefaultBlitSpeed");
		ret = App->parser->UnloadObject();
	}
	else
		ret = false;

	return ret;
}

bool ModuleRender::GetGLError() const
{
	bool ret = true;

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		APPLOG("Error during OpenGP init: %s", gluErrorString(err));
		ret = false;
	}

	return ret;
}

void ModuleRender::LoadPrimitivesGeometry()
{
	//Load Cube
	float3 A = { -0.5f, -0.5f, 0.5f };
	float3 B = { 0.5f, -0.5f, 0.5f };
	float3 C = { -0.5f, 0.5f, 0.5f };
	float3 D = { 0.5f, 0.5f, 0.5f };
	float3 E = { -0.5f, -0.5f, -0.5f };
	float3 F = { 0.5f, -0.5f, -0.5f };
	float3 G = { -0.5f, 0.5f, -0.5f };
	float3 H = { 0.5f, 0.5f, -0.5f };

	float vertices[] = { A.x, A.y, A.z,  B.x, B.y, B.z,  C.x, C.y, C.z,  D.x, D.y, D.z, E.x, E.y, E.z, F.x, F.y, F.z, G.x, G.y, G.z, H.x, H.y, H.z, A.x, A.y, A.z,  B.x, B.y, B.z, G.x, G.y, G.z, H.x, H.y, H.z };
	unsigned int num_vertices = 12;
	float texture_coord[] = { 0,0, 1,0, 0,1, 1,1, 1,0, 0,0, 1,1, 0,1, 1,1, 0,1, 0,0, 1,0 };
	float check_coord[] = { 0,0, 4,0, 0,4, 4,4, 4,0, 0,0, 4,4, 0,4, 4,4, 0,4, 0,0, 4,0 };

	float normals[] = { -1,-1,1, 1,-1,1, -1,1,1, 1,1,1, -1,-1,-1, 1,-1,-1, -1,1,-1, 1,1,-1, -1,-1,1, 1,-1,1, -1,1,-1, 1,1,-1 };

	unsigned triangles_indices[] = { 0, 1, 2,  2, 1, 3,  3, 9, 5,  3, 5, 11,  10, 4, 8,  10, 8, 2,  2, 3, 10,  10, 3, 11,  6, 7, 4,  4, 7, 5,  4, 5, 8,  8, 5, 9 };
	num_indices_triangles = 36;

	Primitives::Cube.Init(num_vertices, num_indices_triangles, vertices, texture_coord, normals, triangles_indices);

	glGenBuffers(1, (GLuint*) &(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * num_vertices, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(id_texture_check));
	glBindBuffer(GL_ARRAY_BUFFER, id_texture_check);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * num_vertices, check_coord, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(id_texture));
	glBindBuffer(GL_ARRAY_BUFFER, id_texture);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * num_vertices, texture_coord, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(id_normals));
	glBindBuffer(GL_ARRAY_BUFFER, id_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_indices_triangles, normals, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(id_indices_triangles));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices_triangles);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * num_indices_triangles, triangles_indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}