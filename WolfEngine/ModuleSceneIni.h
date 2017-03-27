#ifndef MODULESCENEINI_H
#define MODULESCENEINI_H

#include "Module.h"
#include "Globals.h"

class GameObject;
class MyQuadTree;
class Billboard;

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
	const char* anim_idle = "ArmyPilot_Idle";
	const char* anim_walk = "ArmyPilot_Walk";
	const char* anim_run = "ArmyPilot_Run_Forwards";
	MyQuadTree* quad_tree = nullptr;
	Billboard* grass = nullptr;
};

#endif