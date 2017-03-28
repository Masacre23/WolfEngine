#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "SDL/include/SDL.h"
#include "JsonHandler.h"

#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ModuleAudio::ModuleAudio() : Module(MODULE_AUDIO)
{
}

ModuleAudio::~ModuleAudio()
{
}

bool ModuleAudio::Init()
{
	APPLOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if (ConstantConfig() == false)
	{
		APPLOG("Problem retrieving value from configuration file");
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		APPLOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		APPLOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		ret = false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		APPLOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		ret = false;
	}
	else
	{
		Mix_Volume(-1, VOLUME_FX);
		Mix_VolumeMusic(VOLUME_MUSIC);
	}

	return ret;
}

bool ModuleAudio::CleanUp()
{
	APPLOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if (music != nullptr)
		Mix_FreeMusic(music);

	for (std::vector<Mix_Chunk*>::iterator it = fx.begin(); it != fx.end(); ++it)
		Mix_FreeChunk(*it);
	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

bool ModuleAudio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if (music != nullptr)
	{
		if (fade_time > 0.0f)
			Mix_FadeOutMusic((int)(fade_time*1000.0f));
		else
			Mix_HaltMusic();

		Mix_FreeMusic(music);
		music = nullptr;
	}

	music = Mix_LoadMUS(path);

	if (music == nullptr)
	{
		APPLOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if (fade_time > 0.0f)
		{
			if (Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				APPLOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if (Mix_PlayMusic(music, -1) < 0)
			{
				APPLOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	APPLOG("Successfully playing %s", path);
	return ret;
}

void ModuleAudio::StopMusic()
{
	if (music != nullptr)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = nullptr;
	}
}

unsigned int ModuleAudio::LoadFx(const char* path)
{
	unsigned int ret = 0;
	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if (chunk == nullptr)
	{
		APPLOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = fx.size() - 1;
	}

	return ret;
}

bool ModuleAudio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if (id < fx.size())
	{
		Mix_PlayChannel(-1, fx[id], repeat);
		ret = true;
	}

	return ret;
}

bool ModuleAudio::IsPlayingMusic() const 
{ 
	return Mix_PlayingMusic(); 
}

bool ModuleAudio::ConstantConfig()
{
	bool ret = true;

	if (App->parser->LoadObject(AUDIO_SECTION) == true)
	{
		DEFAULT_FADE = App->parser->GetFloat("MusicDefaultFadeTime");
		VOLUME_MUSIC = App->parser->GetInt("MusicVolume");
		VOLUME_FX = App->parser->GetInt("EffectsVolume");
		ret = App->parser->UnloadObject();
	}
	else
		ret = false;

	return ret;
}