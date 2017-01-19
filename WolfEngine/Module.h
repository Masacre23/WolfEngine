#ifndef MODULE_H
#define MODULE_H

#include "Globals.h"

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
				LOG("Module unable to start correctly: %s ---", name);
			return active;
		}
		else
		{
			LOG("Module already active: %s --------------", name);
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
				LOG("Module unable to cleanup correctly: %s -", name);
			return !active;
		}
		else
		{
			LOG("Module already disabled: %s ------------", name);
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

	virtual update_status PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp()
	{
		return true;
	}

private:
	const char* name = "";
	bool active = true;
};

#endif // !MODULE_H