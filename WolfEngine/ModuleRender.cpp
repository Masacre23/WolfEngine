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

Point3df ModuleRender::A = { -0.5f, -0.5f, -0.5f };
Point3df ModuleRender::B = { 0.5f, -0.5f, -0.5f };
Point3df ModuleRender::C = { -0.5f, 0.5f, -0.5f };
Point3df ModuleRender::D = { 0.5f, 0.5f, -0.5f };
Point3df ModuleRender::E = { -0.5f, -0.5f, 0.5f };
Point3df ModuleRender::F = { 0.5f, -0.5f, 0.5f };
Point3df ModuleRender::G = { -0.5f, 0.5f, 0.5f };
Point3df ModuleRender::H = { 0.5f, 0.5f, 0.5f };

int ModuleRender::indices[] = { 0, 1, 2,  2, 1, 3,  3, 1, 5,  3, 5, 7,  6, 4, 0,  6, 0, 2,  2, 3, 6,  6, 3, 7,  6, 7, 4,  4, 7, 5,  4, 5, 0,  0, 5, 1 };
float ModuleRender::vertices[] = { A.x, A.y, A.z,  B.x, B.y, B.z,  C.x, C.y, C.z,  D.x, D.y, D.z, E.x, E.y, E.z, F.x, F.y, F.z, G.x, G.y, G.z, H.x, H.y, H.z };

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

	return ret;
}

bool ModuleRender::Start()
{
	DebugCubeVertices();

	return true;
}

update_status ModuleRender::PreUpdate(float dt)
{
	glViewport(0, 0, App->window->GetScreenWidth(), App->window->GetScreenHeight());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 5.0);

	return UPDATE_CONTINUE;
}

update_status ModuleRender::Update(float dt)
{
	//DebugCube();
	static float angle = 0;
	angle++;
	DrawCube({ 0, 0, -3 }, { 1, 1, 1 }, angle, {0, 1, 1});
	
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

void ModuleRender::DebugCubeVertices()
{
	debug_num_indices = 36;
	debug_num_vertices = 8;

	glGenBuffers(1, (GLuint*) &(debug_id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, debug_id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * debug_num_vertices, vertices, GL_STATIC_DRAW);
	glGenBuffers(1, (GLuint*) &(debug_id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, debug_id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * debug_num_indices, indices, GL_STATIC_DRAW);
}

void ModuleRender::DebugCube()
{
	glMatrixMode(GL_PROJECTION);
	glTranslatef(0.0f, 0.0f, -3.0f);
	glRotatef(angle, 0.0, 1.0, 0.0);
	angle += 0.2f;

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, debug_id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, debug_id_indices);
	glDrawElements(GL_TRIANGLES, debug_num_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleRender::DrawCube(Point3df translate, Point3df scale, float angle, Point3df rotation)
{
	glMatrixMode(GL_PROJECTION);
	glTranslatef(translate.x, translate.y, translate.z);
	glScalef(scale.x, scale.y, scale.z);
	glRotatef(angle, rotation.x, rotation.y, rotation.z);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, debug_id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, debug_id_indices);
	glDrawElements(GL_TRIANGLES, debug_num_indices, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
}
