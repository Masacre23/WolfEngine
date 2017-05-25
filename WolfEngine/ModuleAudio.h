#ifndef MODULEAUDIO_H
#define MODULEAUDIO_H

#include <vector>
#include "Module.h"

#define MODULE_AUDIO "ModuleAudio"
#define AUDIO_SECTION "Config.Modules.Audio"

class ModuleAudio : public Module
{
public:
	ModuleAudio();
	~ModuleAudio();

	bool Init();
	bool CleanUp();

	bool PlayMusic(const char* path) { return PlayMusic(path, DEFAULT_FADE); }
	bool PlayMusic(const char* path, float fade_time);
	void StopMusic();
	unsigned int LoadFx(const char* path);
	bool PlayFx(unsigned int fx, int repeat = 0);

	bool IsPlayingMusic() const;

	bool ConstantConfig();

private:

	float DEFAULT_FADE = 2.0f;
	int VOLUME_MUSIC = 1;
	int VOLUME_FX = 1;
};



#endif // !MODULEAUDIO_H