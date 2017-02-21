#ifndef MODULESCENEINI_H
#define MODULESCENEINI_H

#include "Module.h"
#include "Animation.h"
#include "Globals.h"

class Model;

class ModuleSceneIni : public Module
{
public:
	ModuleSceneIni(bool start_enabled = false);
	~ModuleSceneIni();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:
	Model* batman = nullptr;
	Model* magnetto = nullptr;
};

#endif