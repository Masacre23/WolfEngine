#ifndef MODULEINPUT_H
#define MODULEINPUT_H

#include "Module.h"
#include "Point.h"
#include "SDL/include/SDL_scancode.h"

#define MODULE_INPUT "ModuleInput"
#define INPUT_SECTION "Config.Modules.Input"

class JSONParser;
class SDL_KeyboardEvent;

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	ModuleInput(JSONParser* parser);
	~ModuleInput();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	bool CleanUp();

	KeyState GetKey(int id) const { return keyboard[id]; }
	KeyState GetMouseButtonDown(int id) const { return mouse_buttons[id - 1]; }
	bool GetWindowEvent(EventWindow code) const { return bwindowEvents[code]; }
	const iPoint& GetMouseMotion() const { return mouse_motion; }
	const iPoint& GetMousePosition() const { return mouse_position; }
	const iPoint& GetMouseWheel() const { return mouse_wheel; }

	void PrintKeyInfo(SDL_KeyboardEvent *key);
	//void ModuleInput::PrintModifiers(SDLMod mod);

public:
	iPoint mouse_motion;
	iPoint mouse_position;
	iPoint mouse_wheel;

private:
	bool bwindowEvents[WE_COUNT];
	KeyState* keyboard;
	KeyState* mouse_buttons;
	int MAX_KEYS;
	int NUM_BUTTONS;
};

#endif // !MODULEINPUT_H