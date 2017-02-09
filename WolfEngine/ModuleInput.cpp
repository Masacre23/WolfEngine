#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"
#include "JsonHandler.h"

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
		switch (event_general.type)
		{
		case SDL_QUIT:
			bwindowEvents[WE_QUIT] = true;
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