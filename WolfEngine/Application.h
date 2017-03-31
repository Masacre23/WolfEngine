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

private:
	void EndUpdate();

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
	
	ModuleEditor* editor;
	ModuleSceneIni* scene_ini;

	JSONParser* parser;
	int frames_last_sec = 0;
	Uint32 last_frame_ms;
	float prev_time = 0;
private:
	std::list<Module*> modules;
	Timer app_timer;
	int total_frames = 0;
	int frames_count = 0;

	Uint32 fps_cap = 60;
	Uint32 cap_ms = 1000;
};

extern Application* App;

#endif // !APPLICATION_H
