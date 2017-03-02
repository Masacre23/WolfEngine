#ifndef COMPONENT_H
#define COMPONENT_H

#include "Globals.h"

enum TypeComponent
{
	TRANSFORM = 0,
	MESH,
	MATERIAL,
};

class Component
{

public:
	Component(TypeComponent type) : type(type) {}
	virtual ~Component() {}

	virtual void Enable() {
		enable = true;
	}
	virtual bool OnUpdate() { return true; }
	virtual bool OnEditor() { return true; }
	virtual void Disable() {
		enable = false;
	}


protected: 
	TypeComponent type;
	bool enable = true;
};

#endif

