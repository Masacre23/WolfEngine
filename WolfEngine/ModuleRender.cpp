#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleTextures.h"
#include "ModuleEditor.h"
#include "ModuleLevel.h"
#include "ModulePhysics.h"
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
{
}

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

		SetVsync(vsync);

		ret = ret && GetGLError();
	}

	debug_drawer = new RenderDebugDrawer();

	return ret;
}

bool ModuleRender::Start()
{
	base_plane = new PrimitivePlane(200.0f, 0.0f, 1.0f, Colors::White);

	return true;
}

update_status ModuleRender::PreUpdate(float dt)
{
	BROFILER_CATEGORY("ModuleRender-PreUpdate", Profiler::Color::Blue);

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
	BROFILER_CATEGORY("ModuleRender-Update", Profiler::Color::Red);

	if (draw_debug)
	{
		debug_drawer->PreDebugDraw();
		base_plane->Draw();
		debug_drawer->PostDebugDraw();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate(float dt)
{
	BROFILER_CATEGORY("ModuleRender-PostUpdate", Profiler::Color::Green);

	App->level->Draw();

	if (draw_debug)
	{
		debug_drawer->PreDebugDraw();
		App->level->DrawDebug();
		App->physics->DrawDebug();
		debug_drawer->PostDebugDraw();
	}

	App->editor->Draw();

	SDL_GL_SwapWindow(App->window->GetWindow());

	return UPDATE_CONTINUE;
}

bool ModuleRender::CleanUp()
{
	APPLOG("Destroying 3d renderer");

	if (glcontext != NULL)
		SDL_GL_DeleteContext(glcontext);

	RELEASE(base_plane);

	RELEASE(debug_drawer);

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

bool ModuleRender::SetVsync(bool active)
{
	vsync = active;
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
		vsync = App->parser->GetBoolMandatory("Vsync");
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