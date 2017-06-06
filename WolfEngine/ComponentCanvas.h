#ifndef COMPONENTCANVAS_H
#define COMPONENTCANVAS_H

#include "Component.h"

class ComponentCanvas : public Component
{
public:
	ComponentCanvas(GameObject* parent);
	~ComponentCanvas();

	void OnDraw() const;
	//void OnDebugDraw() const;
	bool OnEditor();
};

#endif