#ifndef COMPONENT_H
#define COMPONENT_H

#include "Globals.h"

class GameObject;

class Component
{
public:
	enum Type
	{
		TRANSFORM = 0,
		MESH,
		MATERIAL,
		CAMERA,
		ANIMATION,
		UNKNOWN
	};

public:
	Component(Type type, GameObject* parent) : type(type), parent(parent) {}
	virtual ~Component() {}

	virtual void Enable() { enable = true; }
	virtual bool OnUpdate() { return true; }
	virtual bool OnDraw() const { return true; }
	virtual bool OnEditor(int selection_mask, int id) { return true; }
	virtual void Disable() { enable = false; }

	Type GetType() const { return type; }
	GameObject* GetParent() const { return parent; }
	bool IsActive() const { return enable; }

protected:
	bool enable = true;
	Type type;
	GameObject* parent;
};

#endif

