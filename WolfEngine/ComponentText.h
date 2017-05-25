#ifndef COMPONENTTEXT_H
#define COMPONENTTEXT_H

#include "Component.h"

class ComponentText :
	public Component
{
public:
	ComponentText(GameObject* parent);
	~ComponentText();

	void OnDraw() const;
	//void OnDebugDraw() const;
	bool OnEditor();

public:
	char* text = "Holiii";
};

#endif