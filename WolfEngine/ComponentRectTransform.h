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
	int posx;
	int posy;
	int sizex;
	int sizey;

	bool draggable;
	bool invisible;
	bool cut_childs;
	bool interactive;
};

#endif