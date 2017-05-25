#ifndef COMPONENTANIM_H
#define COMPONENTANIM_H

#include "Component.h"

class ComponentAudioSource : public Component
{
public:
	ComponentAudioSource(GameObject* parent);
	~ComponentAudioSource();

	bool OnEditor();
	void SaveComponent();
	void RestoreComponent();

	void LoadSound(const char* sound);

	void PlaySound();
	void StopSound();
};

#endif

