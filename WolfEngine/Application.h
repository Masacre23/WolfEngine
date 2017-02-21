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
class ModuleAudio;

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
	ModuleCamera* camera;
	ModuleTextures* textures;
	ModuleAudio* audio;
	
	ModuleEditor* editor;
	ModuleSceneIni* scene_ini;

	JSONParser* parser;
	int frames_last_sec = 0;
	Uint32 last_frame_ms;
	float prev_time = 0;
private:
	std::list<Module*> modules;
	Timer app_timer;
	Timer update_timer;
	int total_frames = 0;
	int frames_count = 0;

	Uint32 fps_cap = 60;
	Uint32 cap_ms = 1000;
	float dt;
};

extern Application* App;

#endif // !APPLICATION_H
