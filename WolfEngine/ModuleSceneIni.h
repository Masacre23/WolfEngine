#ifndef MODULESCENEINI_H
#define MODULESCENEINI_H

#include "Module.h"
#include "Globals.h"
#include <vector>

class GameObject;
class MyQuadTree;
class Grass;

class ModuleSceneIni : public Module
{
public:
	ModuleSceneIni(bool start_enabled = false);
	~ModuleSceneIni();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:
	GameObject* pilot = nullptr;
	std::vector<GameObject*> empty_game_objects;
	const char* anim_idle = "ArmyPilot_Idle";
	const char* anim_walk = "ArmyPilot_Walk";
	const char* anim_run = "ArmyPilot_Run_Forwards";
	MyQuadTree* quad_tree = nullptr;
	Grass* grass = nullptr;
};

#endif