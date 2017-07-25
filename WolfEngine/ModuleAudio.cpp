#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "Bass.h"

ModuleAudio::ModuleAudio() : Module(MODULE_AUDIO)
{
}

ModuleAudio::~ModuleAudio()
{
}

bool ModuleAudio::Init()
{
	APPLOG("Loading Audio Mixer");
	
	BASS_Init(-1, 44100, 0, 0, NULL);
	BASS_SetVolume(1);
	HSAMPLE sample = BASS_SampleLoad(false, "Resources/Audio/batman.ogg", 0, 0, 1, BASS_SAMPLE_MONO);
	HCHANNEL channel = BASS_SampleGetChannel(sample, FALSE);

	//BASS_ChannelPlay(channel, FALSE);
	return true;
}

bool ModuleAudio::CleanUp()
{
	APPLOG("Freeing sound FX, closing Mixer and Audio subsystem");

	BASS_Free();

	return true;
}

bool ModuleAudio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;
	//BASS_ChannelPlay(channel, FALSE);
	return ret;
}

void ModuleAudio::StopMusic()
{
	
}

unsigned int ModuleAudio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	return ret;
}

bool ModuleAudio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	return ret;
}

bool ModuleAudio::IsPlayingMusic() const 
{ 
	return false; 
}

bool ModuleAudio::ConstantConfig()
{
	bool ret = true;

	return ret;
}