#ifndef __MODULESCENEINI_H__
#define __MODULESCENEINI_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"

struct SDL_Texture;

class ModuleSceneIni : public Module
{
public:
	ModuleSceneIni(bool start_enabled = true);
	~ModuleSceneIni();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	SDL_Texture* graphics = nullptr;
	SDL_Rect* cat;
};

#endif