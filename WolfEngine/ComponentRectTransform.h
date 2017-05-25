#ifndef COMPONENTRECTTRANSFORM_H
#define COMPONENTRECTTRANSFORM_H

#include "Component.h"

class ComponentRectTransform : public Component
{
public:
	ComponentRectTransform(GameObject * parent);
	~ComponentRectTransform();

	void OnDraw() const;
	//void OnDebugDraw() const;
	bool OnEditor();

public:
	float pos[2] = {400, 400};
	float sizex;
	float sizey;

	bool draggable;
	bool invisible;
	bool cut_childs;
	bool interactive;
};

#endif