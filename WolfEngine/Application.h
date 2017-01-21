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
class ModuleTextures;
class ModuleAudio;

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
	ModuleTextures* textures;
	ModuleAudio* audio;
	
	ModuleSceneIni* scene_ini;

	JSONParser* parser;

private:
	std::list<Module*> modules;
	Timer timer;
	int total_frames = 0;
	int frames_count = 0;
	int frames_last_sec = 0;
	float prev_time = 0;
	Uint32 fps_cap = 60;
	float dt;

};

extern Application* App;

#endif // !APPLICATION_H
