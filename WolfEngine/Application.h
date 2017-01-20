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

	JSONParser* parser;

private:
	std::list<Module*> modules;
	Timer timer;
	int total_frames;
	int frames;
	int time = 0;
	int prev_time = 0;
	int frames_last_sec = 0;
	Uint32 fps_cap;
	float dt;

};

extern Application* App;

#endif // !APPLICATION_H
