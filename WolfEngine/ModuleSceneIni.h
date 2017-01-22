#ifndef MODULESCENEINI_H
#define MODULESCENEINI_H

#include "Module.h"
#include "Animation.h"
#include "Globals.h"

struct SDL_Texture;

class ModuleSceneIni : public Module
{
public:
	ModuleSceneIni(bool start_enabled = false);
	~ModuleSceneIni();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:
	SDL_Texture* graphics = nullptr;

};

#endif