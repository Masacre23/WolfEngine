#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "Bass.h"

static const char* BASS_GetErrorString()
{
	switch (BASS_ErrorGetCode())
	{
	case -1: return "mystery problem";
	case 0: return "all is OK";
	case 1: return "memory error";
	case 2: return "can't open the file";
	case 3: return "can't find a free/valid driver";
	case 4: return "the sample buffer was lost";
	case 5: return "invalid handle";
	case 6: return "unsupported sample format";
	case 7: return "invalid position";
	case 8: return "BASS_Init has not been successfully called";
	case 9: return "BASS_Start has not been successfully called";
	case 10: return "SSL/HTTPS support isn't available";
	case 14: return "already initialized/paused/whatever";
	case 18: return "can't get a free channel";
	case 19: return "an illegal type was specified";
	case 20: return "an illegal parameter was specified";
	case 21: return "no 3D support";
	case 22: return "no EAX support";
	case 23: return "illegal device number";
	case 24: return "not playing";
	case 25: return "illegal sample rate";
	case 27: return "the stream is not a file stream";
	case 29: return "no hardware voices available";
	case 31: return "the MOD music has no sequence data";
	case 32: return "no internet connection could be opened";
	case 33: return "couldn't create the file";
	case 34: return "effects are not available";
	case 37: return "requested data is not available";
	case 38: return "the channel is/isn't a 'decoding channel'";
	case 39: return "a sufficient DirectX version is not installed";
	case 40: return "connection timedout";
	case 41: return "unsupported file format";
	case 42: return "unavailable speaker";
	case 43: return "invalid BASS version (used by add-ons)";
	case 44: return "codec is not available or supported";
	case 45: return "the channel/file has ended";
	case 46: return "the device is busy";
	default: return "unknown error code";
	}
}

ModuleAudio::ModuleAudio() : Module(MODULE_AUDIO)
{
}

ModuleAudio::~ModuleAudio()
{
	CleanUp();
}

bool ModuleAudio::Init()
{
	bool ret = true;
	APPLOG("Loading Audio Mixer");
	
	BASS_Init(-1, 44100, 0, 0, NULL);
	HSAMPLE sample = BASS_SampleLoad(false, "Resources/Audio/batman.ogg", 0, 0, 1, BASS_SAMPLE_MONO);
	//BASS_SetVolume(0);
	HCHANNEL channel = BASS_SampleGetChannel(sample, FALSE);

	if (BASS_Init(-1, 44100, BASS_DEVICE_3D, 0, NULL) != TRUE)
	{
		APPLOG("BASS_Init() error: %s", BASS_GetErrorString());
		ret = false;
	}

	return ret;
}

bool ModuleAudio::CleanUp()
{
	APPLOG("Freeing sound FX, closing Mixer and Audio subsystem");
	BASS_Free();

	return true;
}

update_status ModuleAudio::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

unsigned int ModuleAudio::LoadFx(const char* path)
{
	unsigned long ret = 0;

	// WAV for samples
	HSAMPLE sample = BASS_SampleLoad(false, path, 0, 0, 5, BASS_SAMPLE_OVER_VOL);

	if (sample == 0) 
	{
		APPLOG("BASS_SampleLoad() file [%s] error: %s", path, BASS_GetErrorString());
	}
	else
	{
		ret = BASS_SampleGetChannel(sample, FALSE);

		if (ret == 0)
			APPLOG("BASS_SampleGetChannel() with id [%ul] error: %s", sample, BASS_GetErrorString());
	}

	return ret;
}

void ModuleAudio::UnloadFx(unsigned long channel)
{
	if (channel != 0)
	{
		BASS_CHANNELINFO info;
		BASS_ChannelGetInfo(channel, &info);
		if (info.filename != nullptr)
			BASS_SampleFree(channel);
		else
			BASS_StreamFree(channel);
	}
}

