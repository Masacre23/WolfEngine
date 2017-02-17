#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"
#include "JsonHandler.h"
#include "ModuleEditor.h"

ModuleInput::ModuleInput(JSONParser* parser) : Module(MODULE_INPUT), mouse_position({0,0}), mouse_motion({0,0})
{
	assert(parser != nullptr);

	if (parser->LoadObject(INPUT_SECTION))
	{
		MAX_KEYS = parser->GetInt("KeyboardKeys");
		keyboard = new KeyState[MAX_KEYS];
		memset(keyboard, KEY_IDLE, MAX_KEYS * sizeof(KeyState));

		NUM_BUTTONS = parser->GetInt("MouseButtons");
		mouse_buttons = new KeyState[NUM_BUTTONS];
		memset(mouse_buttons, KEY_IDLE, NUM_BUTTONS * sizeof(KeyState));
	}
	parser->UnloadObject();

}

ModuleInput::~ModuleInput()
{
	RELEASE_ARRAY(keyboard);
	RELEASE_ARRAY(mouse_buttons);
}

bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleInput::Start()
{
	return true;
}

update_status ModuleInput::PreUpdate(float dt)
{
	static SDL_Event event_general;

	mouse_motion = { 0, 0 };
	mouse_wheel = { 0, 0 };
	memset(bwindowEvents, false, WE_COUNT * sizeof(bool));

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	while (SDL_PollEvent(&event_general) != 0)
	{
		App->editor->HandleInput(&event_general);
		switch (event_general.type)
		{
		case SDL_QUIT:
			bwindowEvents[WE_QUIT] = true;
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			PrintKeyInfo(&event_general.key);
			break;
		case SDL_WINDOWEVENT:
			switch (event_general.window.event)
			{
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				bwindowEvents[WE_HIDE] = true;
				break;
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				bwindowEvents[WE_SHOW] = true;
				break;

			case SDL_WINDOWEVENT_SIZE_CHANGED:
				App->window->WindowResize(event_general.window.data1, event_general.window.data2);
				break;
			}
			
		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event_general.button.button - 1] = KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event_general.button.button - 1] = KEY_UP;
			break;

		case SDL_MOUSEMOTION:
			mouse_motion.x = event_general.motion.xrel;
			mouse_motion.y = event_general.motion.yrel;
			mouse_position.x = event_general.motion.x;
			mouse_position.y = event_general.motion.y;
			break;
		case SDL_MOUSEWHEEL:
			mouse_wheel.y = event_general.wheel.y;
			break;
		}
	}

	if (GetWindowEvent(EventWindow::WE_QUIT) == true || GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

/* Print all information about a key event */
void ModuleInput::PrintKeyInfo(SDL_KeyboardEvent *key) {
	/* Is it a release or a press? */
	/*if (key->type == SDL_KEYUP)
		printf("Release:- ");
	else
		printf("Press:- ");*/

	/* Print the hardware scancode first */
	//printf("Scancode: 0x%02X", key->keysym.scancode);
	/* Print the name of the key */
	//printf(", Name: %s", SDL_GetKeyName(key->keysym.sym));
	/* We want to print the unicode info, but we need to make */
	/* sure its a press event first (remember, release events */
	/* don't have unicode info                                */
	//if (key->type == SDL_KEYDOWN) {
		/* If the Unicode value is less than 0x80 then the    */
		/* unicode value can be used to get a printable       */
		/* representation of the key, using (char)unicode.    */
	//	printf(", Unicode: ");
	//	if (key->keysym.unicode < 0x80 && key->keysym.unicode > 0) {
	//		printf("%c (0x%04X)", (char)key->keysym.unicode,
	//			key->keysym.unicode);
	//	}
	//	else {
	//		printf("? (0x%04X)", key->keysym.unicode);
	//	}
	//}
	//printf("\n");
	/* Print modifier info */
	//PrintModifiers(key->keysym.mod);
	App->editor->AddLog(SDL_GetKeyName(key->keysym.sym));
	//PrintModifiers(key->keysym.mod);
}

/* Print modifier info */
/*void ModuleInput::PrintModifiers(SDLMod mod) {
	printf("Modifers: ");

	/* If there are none then say so and return */
	/*if (mod == KMOD_NONE) {
		printf("None\n");
		return;
	}

	/* Check for the presence of each SDLMod value */
	/* This looks messy, but there really isn't    */
	/* a clearer way.                              */
	/*if (mod & KMOD_NUM) printf("NUMLOCK ");
	if (mod & KMOD_CAPS) printf("CAPSLOCK ");
	if (mod & KMOD_LCTRL) printf("LCTRL ");
	if (mod & KMOD_RCTRL) printf("RCTRL ");
	if (mod & KMOD_RSHIFT) printf("RSHIFT ");
	if (mod & KMOD_LSHIFT) printf("LSHIFT ");
	if (mod & KMOD_RALT) printf("RALT ");
	if (mod & KMOD_LALT) printf("LALT ");
	if (mod & KMOD_CTRL) printf("CTRL ");
	if (mod & KMOD_SHIFT) printf("SHIFT ");
	if (mod & KMOD_ALT) printf("ALT ");
	printf("\n");
}*/