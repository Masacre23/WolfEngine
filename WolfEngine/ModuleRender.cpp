#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "SDL/include/SDL.h"
#include "OpenGL.h"
#include "JsonHandler.h"

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
	LOG("Creating 3D Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if (ConstantConfig() == false)
	{
		LOG("Problem retrieving value from configuration file");
		ret = false;
	}

	//Creating OpenGL Context for SDL
	glcontext = SDL_GL_CreateContext(App->window->GetWindow());
	if (glcontext == NULL)
	{
		LOG("GL Context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	//Initialize Glew
	GLenum err = glewInit();
	if (err != GL_NO_ERROR)
	{
		LOG("Error during Glew library init: %s\n", glewGetErrorString(err));
		ret = false;
	}
	else
		LOG("Using Glew %s", glewGetString(GLEW_VERSION));

	if (ret == true)
	{
		//Get hardware and drivers capabilities
		LOG("Vendor: %s", glGetString(GL_VENDOR));
		LOG("Renderer: %s", glGetString(GL_RENDERER));
		LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

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

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		ret = ret && GetGLError();
	}

	camera.w = App->window->GetScreenWidth() * SCREENSIZE;
	camera.h = App->window->GetScreenHeight() * SCREENSIZE;

	return ret;
}

update_status ModuleRender::PreUpdate(float dt)
{
	glViewport(0, 0, App->window->GetScreenWidth(), App->window->GetScreenHeight());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 5.0);

	return UPDATE_CONTINUE;
}

update_status ModuleRender::Update(float dt)
{
	DebugCamera(dt);

	glBegin(GL_TRIANGLES);
	glVertex3f(-1.0f, -0.5f, -4.0f); // lower left vertex
	glVertex3f(1.0f, -0.5f, -4.0f); // lower right vertex
	glVertex3f(0.0f, 0.5f, -4.0f); // upper vertex
	glEnd();

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate(float dt)
{
	SDL_GL_SwapWindow(App->window->GetWindow());

	return UPDATE_CONTINUE;
}

bool ModuleRender::CleanUp()
{
	LOG("Destroying 3d renderer");

	if (glcontext != NULL)
		SDL_GL_DeleteContext(glcontext);

	return true;
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

	SCREENSIZE = App->window->GetScreenSize();
	SCREENWIDTH = App->window->GetScreenWidth();
	SCREENHEIGHT = App->window->GetScreenHeight();

	return ret;
}

bool ModuleRender::GetGLError() const
{
	bool ret = true;

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		LOG("Error during OpenGP init: %s", gluErrorString(err));
		ret = false;
	}

	return ret;
}

void ModuleRender::DebugCamera(float dt)
{
	// debug camera
	int speed = 300;
	
	if (App->input->GetKey(SDL_SCANCODE_KP_8) == KEY_REPEAT)
		App->renderer->camera.y += ceil(speed*dt);

	if (App->input->GetKey(SDL_SCANCODE_KP_2) == KEY_REPEAT)
		App->renderer->camera.y -= ceil(speed*dt);

	if (App->input->GetKey(SDL_SCANCODE_KP_4) == KEY_REPEAT)
		App->renderer->camera.x += ceil(speed*dt);

	if (App->input->GetKey(SDL_SCANCODE_KP_6) == KEY_REPEAT)
		App->renderer->camera.x -= ceil(speed*dt);
}
