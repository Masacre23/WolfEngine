#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "JsonHandler.h"
#include "TimerUs.h"
#include "ModuleSceneIni.h"

Application::Application()
{
	parser = new JSONParser(CONFIGJSON);

	modules.push_back(input = new ModuleInput(parser));
	modules.push_back(window = new ModuleWindow());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());

	modules.push_back(scene_ini = new ModuleSceneIni());
	scene_ini->Enable();

	if (parser->LoadObject(APP_SECTION))
	{
		//Get fps. 
		fps_cap = parser->GetInt("FpsCap");
		//Change fps to mspf
		fps_cap = 1000 / fps_cap;
		parser->UnloadObject();
	}
}

Application::~Application()
{
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);

	RELEASE(parser);
}

bool Application::Init()
{
	bool ret = true;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if ((*it)->IsEnabled())
			ret = (*it)->Start();
	}

	timer.Start();
	frames = 0;
	total_frames = 0;
	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	Timer updateTimer;
	TimerUs* delay_timer = new TimerUs();
	updateTimer.Start();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled())
			ret = (*it)->PreUpdate();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled())
			ret = (*it)->Update(dt);

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled())
			ret = (*it)->PostUpdate();

	// TODO 4
	// Amount of frames since startup
	total_frames++;
	frames++;
	LOG("Total frames: %i", total_frames);

	// Amount of time since game start
	time = timer.GetTimeInMs() / 1000;
	//LOG("Time: %i s", time);

	// Average FPS for the whole game life.
	if(time!=0)
		LOG("Average FPS: %i", total_frames / time);

	// Amount of ms took the last update.
	LOG("Update time: %i ms", updateTimer.GetTimeInMs());

	// Amount of frames during the last second (the actual FPS)
	if (time > prev_time)
	{
		frames_last_sec = frames;
		frames = 0;
		prev_time++;
	}
	LOG("Frames last second: %i", frames_last_sec);
	delay_timer->Start();
	int i = 0;
	Uint64 dt = delay_timer->GetTimeInUs();
	//Calculate the time for the next frame.
	if (updateTimer.GetTimeInMs() < fps_cap) {
		Uint32 time_to_nframe = fps_cap - updateTimer.GetTimeInMs();
		delay_timer->Start();
		Timer::DelayInMs(time_to_nframe);
		float real_delay_time = (float)delay_timer->GetTimeInUs() / 1000;
		LOG("We wait for %i milliseconds and got back in %f", time_to_nframe, real_delay_time);
	}
	RELEASE(delay_timer);

	// TODO 6
	//  differential time since last frame 
	dt = (float)updateTimer.GetTimeInMs()/1000;
	LOG("dt: %f", dt);

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if ((*it)->IsEnabled())
			ret = (*it)->CleanUp();

	return ret;
}