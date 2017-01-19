#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "SDL/include/SDL.h"
#include "JsonHandler.h"

ModuleRender::ModuleRender() : Module(MODULE_RENDER)
{
}

ModuleRender::~ModuleRender()
{}

bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if (ConstantConfig() == false)
	{
		LOG("Problem retrieving value from configuration file");
		ret = false;
	}

	camera.w = App->window->GetScreenWidth() * SCREENSIZE;
	camera.h = App->window->GetScreenHeight() * SCREENSIZE;

	if (VSYNC)
		flags |= SDL_RENDERER_PRESENTVSYNC;

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);

	if (renderer == nullptr)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_RenderPresent(renderer);
	return UPDATE_CONTINUE;
}

bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	if (renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

bool ModuleRender::BlitScreenCentered(SDL_Texture* texture, SDL_Rect* section, float speed)
{
	iPoint draw_origin;

	if (section != nullptr)
	{
		draw_origin.x = (SCREENWIDTH - section->w) / 2;
		draw_origin.y = (SCREENHEIGHT - section->h) / 2;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &draw_origin.x, &draw_origin.y);
		draw_origin.x = (SCREENWIDTH - draw_origin.x) / 2;
		draw_origin.y = (SCREENHEIGHT - draw_origin.y) / 2;
	}

	return Blit(texture, draw_origin, section, speed);
}

bool ModuleRender::BlitScreenXCentered(SDL_Texture * texture, int y, SDL_Rect * section, float speed)
{
	iPoint draw_origin;

	if (section != nullptr)
	{
		draw_origin.x = (SCREENWIDTH - section->w) / 2;
		draw_origin.y = y;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &draw_origin.x, &draw_origin.y);
		draw_origin.x = (SCREENWIDTH - draw_origin.x) / 2;
		draw_origin.y = y;
	}

	return Blit(texture, draw_origin, section, speed);
}

bool ModuleRender::Blit(SDL_Texture* texture, const iPoint& position, SDL_Rect* section, float speed, bool inverse)
{
	bool ret = true;
	SDL_Rect rect;
	rect.x = position.x * SCREENSIZE + (int)(camera.x * speed);
	rect.y = position.y * SCREENSIZE + (int)(camera.y * speed);

	if (section != nullptr)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

	rect.w *= SCREENSIZE;
	rect.h *= SCREENSIZE;

	if (inverse == false)
	{
		if (SDL_RenderCopy(renderer, texture, section, &rect) != 0)
		{
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}
	}
	else
	{
		if (SDL_RenderCopyEx(renderer, texture, section, &rect, 0.0, NULL, SDL_FLIP_HORIZONTAL) != 0)
		{
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}
	}
	

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera == true)
	{
		rec.x = (int)(camera.x + rect.x * SCREENSIZE);
		rec.y = (int)(camera.y + rect.y * SCREENSIZE);
		rec.w *= SCREENSIZE;
		rec.h *= SCREENSIZE;
	}

	if (SDL_RenderFillRect(renderer,&rec) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

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

	SCREENSIZE = App->window->GetScreenSize();
	SCREENWIDTH = App->window->GetScreenWidth();
	SCREENHEIGHT = App->window->GetScreenHeight();

	return ret;
}

void ModuleRender::DebugCamera()
{
	// debug camera
	int speed = 5;
	
	if (App->input->GetKey(SDL_SCANCODE_KP_8) == KEY_REPEAT)
		App->renderer->camera.y += speed;

	if (App->input->GetKey(SDL_SCANCODE_KP_2) == KEY_REPEAT)
		App->renderer->camera.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_KP_4) == KEY_REPEAT)
		App->renderer->camera.x += speed;

	if (App->input->GetKey(SDL_SCANCODE_KP_6) == KEY_REPEAT)
		App->renderer->camera.x -= speed;
}
