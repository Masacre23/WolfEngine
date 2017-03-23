#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleTextures.h"
#include "ModuleEditor.h"
#include "SDL/include/SDL.h"
#include "OpenGL.h"
#include "Math.h"
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
	LoadCubeGeometry();

	return true;
}

update_status ModuleRender::PreUpdate(float dt)
{
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
	glEnable(GL_COLOR_MATERIAL);
	DrawBasePlane();
	DrawAxis();
	glDisable(GL_COLOR_MATERIAL);

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate(float dt)
{
	App->editor->Draw();
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

void ModuleRender::LoadCubeGeometry()
{
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

	int triangles_indices[] = { 0, 1, 2,  2, 1, 3,  3, 9, 5,  3, 5, 11,  10, 4, 8,  10, 8, 2,  2, 3, 10,  10, 3, 11,  6, 7, 4,  4, 7, 5,  4, 5, 8,  8, 5, 9 };
	num_indices_triangles = 36;

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

void ModuleRender::DrawCube(unsigned int texture, float3 translate, float3 scale, float angle, float3 rotation)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	GLuint id_texture;
	if (texture == App->textures->texture_checkers)
		id_texture = id_texture_check;
	else
		id_texture = this->id_texture;

	glBindTexture(GL_TEXTURE_2D, texture);

	glColor3f(1.0f, 1.0f, 1.0f);

	glTranslatef(translate.x, translate.y, translate.z);
	glScalef(scale.x, scale.y, scale.z);
	glRotatef(angle, rotation.x, rotation.y, rotation.z);
	
	glDepthRange(0.1, 0.9);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, id_normals);
	glNormalPointer(GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, id_texture);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices_triangles);
	glDrawElements(GL_TRIANGLES, num_indices_triangles, GL_UNSIGNED_INT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}

void ModuleRender::DrawBasePlane()
{
	float distance_between_lines = 1.0f;
	float min_distance = -100.f;
	float max_distance = 100.f;
	int num_lines = ((int)((max_distance - min_distance) / distance_between_lines)) + 1;

	glLineWidth(2.0f);
	glDepthRange(0.1, 0.9);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < num_lines; i++)
	{
		glVertex3f(min_distance + i*distance_between_lines, 0.0, min_distance);
		glVertex3f(min_distance + i*distance_between_lines, 0.0, max_distance);
	}
	for (int i = 0; i < num_lines; i++)
	{
		glVertex3f(min_distance, 0.0, min_distance + i*distance_between_lines);
		glVertex3f(max_distance, 0.0, min_distance + i*distance_between_lines);
	}
	glEnd();
}

void ModuleRender::DrawAxis()
{
	float axis_length = 1.5f;

	glLineWidth(2.0f);
	glDepthRange(0.0, 0.1);
	glBegin(GL_LINES);

	glColor3f(3.0, 0.0f, 0.0f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(axis_length, 0.0, 0.0);
	
	glColor3f(0.0f, 3.0, 0.0f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, axis_length, 0.0);

	glColor3f(0.0f, 0.0f, 3.0f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, axis_length);

	glColor3f(1.0f, 1.0f, 1.0f);

	glEnd();
}

bool ModuleRender::SetVsync(bool vsync)
{
	bool ret = true;
	if (SDL_GL_SetSwapInterval(vsync ? 1 : 0))
		LOG("Error during Vsync setting: %s", SDL_GetError());
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
		LOG("Error during OpenGP init: %s", gluErrorString(err));
		ret = false;
	}

	return ret;
}