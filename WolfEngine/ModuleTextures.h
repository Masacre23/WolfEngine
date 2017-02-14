#ifndef MODULETEXTURES_H
#define MODULETEXTURES_H

#include <list>
#include "Module.h"

#define MODULE_TEXTURES "ModuleTextures"

struct SDL_Texture;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();
	
	bool Init();
	bool CleanUp();

	unsigned int LoadTexture(const char* path);

private:
	void LoadCheckers();

public:
	unsigned int texture_checkers;
	unsigned int texture_debug;

private:
	
};


#endif // !MODULETEXTURES_H