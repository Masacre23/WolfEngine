#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"

#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment( lib, "Glew/libx86/glew32.lib" )
#pragma comment( lib, "Glew/libx86/glew32s.lib" )

#include "JsonHandler.h"

ModuleWindow::ModuleWindow() : Module(MODULE_WINDOW)
{
}

 ModuleWindow::~ModuleWindow()
{
}

 bool ModuleWindow::Init()
 {
	 LOG("Init SDL window & surface");

	 bool ret = true;

	 if (ConstantConfig() == false)
	 {
		 LOG("Problem retrieving value from configuration file");
		 ret = false;
	 }

	 if (SDL_Init(SDL_INIT_VIDEO) < 0)
	 {
		 LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		 ret = false;
	 }
	 else
	 {
		 int width = SCREENWIDTH * SCREENSIZE;
		 int height = SCREENHEIGHT * SCREENSIZE;
		 Uint32 flags = SDL_WINDOW_SHOWN;

		 if (FULLSCREEN)
			 flags |= SDL_WINDOW_FULLSCREEN;
		 
		 if (RESIZABLE)
			 flags |= SDL_WINDOW_RESIZABLE;

		 if (BORDERLESS)
			 flags |= SDL_WINDOW_BORDERLESS;

		 if (FULLSCREEN_DESKTOP)
			 flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		 flags |= SDL_WINDOW_OPENGL;

		 SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		 SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		 SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		 SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		 SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		 SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		 window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		 glcontext = SDL_GL_CreateContext(window);
		 if (glcontext == NULL)
		 {
			 LOG("GL Context could not be created! SDL_Error: %s\n", SDL_GetError());
			 ret = false;
		 }

		 GLenum err = glewInit();
		 if (err != GL_NO_ERROR)
		 {
			 LOG("Error in glewInit: %d\n", err);
		 }
		 else
		 {
			 LOG("Using Glew %s", glewGetString(GLEW_VERSION));
		 }

		 if (window == nullptr)
		 {
			 LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			 ret = false;
		 }
		 else
		 {
			 screen_surface = SDL_GetWindowSurface(window);
		 }
	 }

	 return ret;
 }

 bool ModuleWindow::Start()
 {
	 title_fps = new char[TITLE_INFO_LENGTH];

	 return true;
 }


 bool ModuleWindow::CleanUp()
 {
	 LOG("Destroying SDL window and quitting all SDL systems");

	 //Destroy window
	 if (window != nullptr)
	 {
		 SDL_DestroyWindow(window);
	 }

	 if (glcontext != NULL)
	 {
		 SDL_GL_DeleteContext(glcontext);
	 }

	 RELEASE_ARRAY(title_fps);

	 //Quit SDL subsystems
	 SDL_Quit();
	 return true;
 }

 void ModuleWindow::SetFPStoWindow(int total_frames, float total_seconds, Uint32 update_ms, int current_fps)
 {
	 int app_name_lenght = sprintf_s(title_fps, TITLE_INFO_LENGTH, TITLE);
	 sprintf_s(title_fps + app_name_lenght, TITLE_INFO_LENGTH - app_name_lenght,
		 ": Total frames: %i, Total time: %f s, Average FPS: %f, Update time: %u ms, Current FPS: %i",
		 total_frames, total_seconds, (float)total_frames / total_seconds, update_ms, current_fps);
	 SDL_SetWindowTitle(window, title_fps);
 }

 bool ModuleWindow::ConstantConfig()
 {
	 bool ret = true;

	 if (App->parser->LoadObject(APP_SECTION) == true)
	 {
		 TITLE = App->parser->GetString("Title");
		 SCREENWIDTH = App->parser->GetInt("Resolution.Width");
		 SCREENHEIGHT = App->parser->GetInt("Resolution.Height");
		 SCREENSIZE = App->parser->GetInt("Resolution.Scale");
		 ret = App->parser->UnloadObject();
	 }
	 else
		 ret = false;

	 if (App->parser->LoadObject(WINDOW_SECTION) == true)
	 {
		 FULLSCREEN = App->parser->GetBoolMandatory("Fullscreen");
		 BORDERLESS = App->parser->GetBoolMandatory("Borderless");
		 RESIZABLE = App->parser->GetBoolMandatory("Resizable");
		 FULLSCREEN_DESKTOP = App->parser->GetBoolMandatory("Fullscreen_Window");
		 TITLE_INFO_LENGTH = App->parser->GetInt("TitleWithInfoLength");
		 ret = ret && App->parser->UnloadObject();
	 }
	 else
		 ret = false;

	 return ret;
 }
