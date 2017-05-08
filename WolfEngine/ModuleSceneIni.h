#ifndef MODULESCENEINI_H
#define MODULESCENEINI_H

#include "Module.h"
#include "Globals.h"
#include <vector>
#include <list>
#include <string>

class GameObject;
class MyQuadTree;

class ModuleSceneIni : public Module
{
public:
	ModuleSceneIni(bool start_enabled = false);
	~ModuleSceneIni();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:
	std::vector<GameObject*> empty_game_objects;
	MyQuadTree* quad_tree = nullptr;
	GameObject* grass = nullptr;
	GameObject* rain = nullptr;
};

#endif