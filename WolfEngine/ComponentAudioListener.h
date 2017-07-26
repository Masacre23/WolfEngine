#ifndef COMPONENTAUDIOLISTENER_H
#define COMPONENTAUDIOLISTENER_H

#include "Component.h"
class ComponentAudioListener : public Component
{

public:
	ComponentAudioListener(GameObject* parent);
	~ComponentAudioListener();

	bool OnEditor();
	void SaveComponent();
	void RestoreComponent();

public:
	float distance = 100.0f;
	float roll_out = 1.0f;
	float doppler = 1.0f;
	
};

#endif
