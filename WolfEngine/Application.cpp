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
#include "Brofiler/include/Brofiler.h"
#pragma comment(lib, "Brofiler/libx86/ProfilerCore32.lib")

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
	modules.push_back(animations = new ModuleAnimations());
	modules.push_back(level = new ModuleLevel());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(program_shaders = new ModuleProgramShaders());
	modules.push_back(audio = new ModuleAudio());

	modules.push_back(scene_ini = new ModuleSceneIni());

	modules.push_back(time_controller = new ModuleTimeController());

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

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	float dt = time_controller->UpdateDeltaTime();

	BROFILER_FRAME("frameName");
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled())
			ret = (*it)->PreUpdate(dt);
			

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled())
			ret = (*it)->Update(dt);
			

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if ((*it)->IsEnabled())
			ret = (*it)->PostUpdate(dt);
			

	time_controller->EndUpdate();

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
