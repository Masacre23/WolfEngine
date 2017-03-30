#ifndef MODULETIMECONTROLLER_H
#define MODULETIMECONTROLLER_H

#include "Module.h"
#include <vector>

class GameObject;

class ModuleTimeController : public Module
{
public:
	ModuleTimeController();
	~ModuleTimeController();

	bool Init();
	void Play() const;
	void Stop() const;
	bool CleanUp();

public:
	std::vector<GameObject*> gameobjects;
};

#endif