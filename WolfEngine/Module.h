#ifndef MODULE_H
#define MODULE_H

#include "Globals.h"
#include "Brofiler/include/Brofiler.h"

class Module
{
public:
	Module(const char* name, bool active = true) : name(name), active(active) {}
	
	virtual ~Module() {}

	bool IsEnabled() const
	{
		return active;
	}

	bool Enable()
	{
		if (active == false)
		{
			active = Start();
			if (active == false)
				APPLOG("Module unable to start correctly: %s ---", name);
			return active;
		}
		else
		{
			APPLOG("Module already active: %s --------------", name);
			return true;
		}

		return true;
	}

	bool Disable()
	{
		if (active == true)
		{
			active = !CleanUp();
			if (active == true)
				APPLOG("Module unable to cleanup correctly: %s -", name);
			return !active;
		}
		else
		{
			APPLOG("Module already disabled: %s ------------", name);
			return true;
		}

		return true;
	}

	virtual bool Init()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		BROFILER_CATEGORY(name, Profiler::Color::Blue);

		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		BROFILER_CATEGORY(name, Profiler::Color::Red);

		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		BROFILER_CATEGORY(name, Profiler::Color::Green);

		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp()
	{
		return true;
	}

public:
	const char* name = "";

private:
	bool active = true;
};

#endif // !MODULE_H