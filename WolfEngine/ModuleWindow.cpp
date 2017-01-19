#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"
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

		 window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

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

 bool ModuleWindow::CleanUp()
 {
	 LOG("Destroying SDL window and quitting all SDL systems");

	 //Destroy window
	 if (window != nullptr)
	 {
		 SDL_DestroyWindow(window);
	 }

	 //Quit SDL subsystems
	 SDL_Quit();
	 return true;
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
		 ret = ret && App->parser->UnloadObject();
	 }
	 else
		 ret = false;

	 return ret;
 }
