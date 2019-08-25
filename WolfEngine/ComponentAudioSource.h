#ifndef COMPONENTAUDIOSOURCE_H
#define COMPONENTAUDIOSOURCE_H

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

public:
	unsigned long channel;
	bool is_2d = false;
	float min_distance = 0.f;
	float max_distance = 0.f;
	int cone_angle_in = 360;
	int cone_angle_out = 360;
	float out_cone_vol = 0.f;
	float fade_in = 1.0f;
	float fade_out = 1.0f;
};

#endif

