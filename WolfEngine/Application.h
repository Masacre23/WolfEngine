#ifndef APPLICATION_H
#define APPLICATION_H

#include <list>
#include "Globals.h"
#include "Module.h"
#include "Timer.h"

#define APP_SECTION "Config.App"

class JSONParser;

class ModuleInput;
class ModuleWindow;
class ModuleRender;
class ModuleCamera;
class ModuleTextures;
class ModuleLevel;
class ModuleAnimations;
class ModuleAudio;
class ModuleTimeController;
class ModuleProgramShaders;

class ModuleEditor;
class ModuleSceneIni;

class Application
{
public:
	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleInput* input;
	ModuleWindow* window;
	ModuleRender* renderer;
	ModuleLevel* level;
	ModuleAnimations* animations;
	ModuleCamera* camera;
	ModuleTextures* textures;
	ModuleAudio* audio;
	ModuleTimeController* time_controller;
	ModuleProgramShaders* program_shaders;
	
	ModuleEditor* editor;
	ModuleSceneIni* scene_ini;

	JSONParser* parser;

private:
	std::list<Module*> modules;

};

extern Application* App;

#endif // !APPLICATION_H
