#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleLevel.h"
#include "ModuleCamera.h"
#include "ModuleTextures.h"
#include "ModuleAnimations.h"
#include "ModuleAudio.h"
#include "JsonHandler.h"
#include "TimerUs.h"
#include "ModuleSceneIni.h"
#include "ModuleEditor.h"
#include "ModuleTimeController.h"
#include "ModuleProgramShaders.h"

Application::Application()
{
	Timer timer_function_ms;
	TimerUs timer_function_us;
	timer_function_ms.Start();
	timer_function_us.Start();

	parser = new JSONParser(CONFIGJSON);

	modules.push_back(input = new ModuleInput(parser));
	modules.push_back(window = new ModuleWindow());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(time_controller = new ModuleTimeController());
	modules.push_back(animations = new ModuleAnimations());
	modules.push_back(level = new ModuleLevel());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(program_shaders = new ModuleProgramShaders());
	modules.push_back(audio = new ModuleAudio());
	

	modules.push_back(scene_ini = new ModuleSceneIni());

	APPLOG("App construction time (by Timer class) in ms: %u", timer_function_ms.GetTimeInMs());
	APPLOG("App construction time (by TimerUs class) in ms: %llu", timer_function_us.GetTimeInMs());
	APPLOG("App construction time (by TimerUs class) in us: %llu", timer_function_us.GetTimeInUs());
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

	Timer timer_function_ms;
	TimerUs timer_function_us;
	timer_function_ms.Start();
	timer_function_us.Start();

	if (parser->LoadObject(APP_SECTION))
	{
		//Get fps. 
		fps_cap = parser->GetInt("FpsCap");
		//Change fps to mspf
		cap_ms = 1000 / fps_cap;
		parser->UnloadObject();
	}

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	APPLOG("App initialization time (by Timer class) in ms: %u", timer_function_ms.GetTimeInMs());
	APPLOG("App initialization time (by TimerUs class) in ms: %llu", timer_function_us.GetTimeInMs());
	APPLOG("App initialization time (by TimerUs class) in us: %llu", timer_function_us.GetTimeInUs());

	timer_function_ms.Start();
	timer_function_us.Start();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if ((*it)->IsEnabled())
			ret = (*it)->Start();
	}

	scene_ini->Enable();

	APPLOG("App starting time (by Timer class) in ms: %u", timer_function_ms.GetTimeInMs());
	APPLOG("App starting time (by TimerUs class) in ms: %llu", timer_function_us.GetTimeInMs());
	APPLOG("App starting time (by TimerUs class) in us: %llu", timer_function_us.GetTimeInUs());

	app_timer.Start();
	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	// TODO 6
	//  differential time since last frame 
	time_controller->UpdateDeltaTime();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled())
			ret = (*it)->PreUpdate(time_controller->delta_time);

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled())
			ret = (*it)->Update(time_controller->delta_time);

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled())
			ret = (*it)->PostUpdate(time_controller->delta_time);

	EndUpdate();

	return ret;
}

void Application::EndUpdate()
{
	last_frame_ms = time_controller->time->GetTimeInMs();

	// TODO 4
	// Amount of frames since startup
	time_controller->frame_count++;
	//LOG("Total frames: %i", total_frames);
	frames_count++;

	// Amount of time since game start
	time_controller->game_time = ((float)app_timer.GetTimeInMs()) / 1000.0f;

	// Average FPS for the whole game life.
	//LOG("Average FPS: %f", ((float)total_frames) / time_s);

	// Amount of ms took the last update.
	//LOG("Update time: %u ms", last_frame_ms);

	// Amount of frames during the last second (the actual FPS)
	if (time_controller->game_time - prev_time > 1.0f)
	{
		frames_last_sec = frames_count;
		frames_count = 0;
		prev_time = time_controller->game_time;
	}

	App->window->SetFPStoWindow(time_controller->frame_count, time_controller->game_time, last_frame_ms, frames_last_sec, time_controller->delta_time);

	//Calculate the time for the next frame.
	TimerUs delay_timer;
	if (last_frame_ms < cap_ms) {
		Uint32 time_to_nframe = cap_ms - last_frame_ms;
		delay_timer.Start();
		Timer::DelayInMs(time_to_nframe);
		float real_delay_time = (float)delay_timer.GetTimeInMs();
		//LOG("We wait for %i milliseconds and got back in %f", time_to_nframe, real_delay_time);
	}
}

bool Application::CleanUp()
{
	bool ret = true;

	for (std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if ((*it)->IsEnabled())
			ret = (*it)->CleanUp();

	return ret;
}
