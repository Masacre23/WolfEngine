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
		BILLBOARD,
		PARTICLE,
		RIGIDBODY,
		RECT_TRANSFORM,
		IMAGE,
		TEXT,
		CANVAS,
		UNKNOWN
	};

public:
	Component(Type type, GameObject* parent) : type(type), parent(parent) {}
	virtual ~Component() {}

	virtual void OnUpdate() {}
	virtual void OnDraw() const {}
	virtual void OnDebugDraw() const {}
	virtual bool OnEditor() { return false; }

	virtual void OnPlay() {}
	virtual void OnStop() {}

	virtual void Enable() { enable = true; }
	virtual void Disable() { enable = false; }
	virtual void DisableOnEditor() { on_editor = false; }

	virtual void SaveComponent() {}
	virtual void RestoreComponent() {}

	Type GetType() const { return type; }
	GameObject* GetParent() const { return parent; }
	bool IsActive() const { return enable; }

protected:
	bool enable = true;
	bool on_editor = true;
	Type type;
	GameObject* parent;
};

#endif
