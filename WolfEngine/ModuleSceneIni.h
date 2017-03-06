#ifndef MODULESCENEINI_H
#define MODULESCENEINI_H

#include "Module.h"
#include "Animation.h"
#include "Globals.h"

class Model;
class GameObject;

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

	GameObject* empty_gameobject = nullptr;
	GameObject* child_gameobject = nullptr;
	GameObject* child_child_gameobject = nullptr;
};

#endif