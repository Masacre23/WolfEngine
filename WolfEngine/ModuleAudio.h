#ifndef MODULEAUDIO_H
#define MODULEAUDIO_H

#include <vector>
#include "Module.h"

#define MODULE_AUDIO "ModuleAudio"
#define AUDIO_SECTION "Config.Modules.Audio"

#define DEFAULT_FADE 2.0f

class ModuleAudio : public Module
{
public:
	ModuleAudio();
	~ModuleAudio();

	bool Init();
	bool CleanUp();
	update_status PostUpdate(float dt);

	unsigned int LoadFx(const char* path);
	void UnloadFx(unsigned long id);

public:

	float volume = 1.0f;
	float volume_music = 1.0f;
	float volume_fx = 1.0f;

private:
	
};



#endif // !MODULEAUDIO_H