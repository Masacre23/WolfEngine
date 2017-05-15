#ifndef COLLIDER_H
#define COLLIDER_H

#include "Math.h"

class Collider
{
public:
	enum Type
	{
		BOX = 0,
		SPHERE,
		CAPSULE,
		UNKNOWN
	};

public:
	Collider(Type type);
	~Collider();

	virtual void OnEditor() {}
	virtual void OnDebugDraw() {}

	Type GetType() { return type; }

protected:
	float4x4 transform = float4x4::identity;
	Type type;
};

class ColliderBox : public Collider
{
public:
	ColliderBox();

	void OnEditor();
	void OnDebugDraw();

private:
	OBB box;
};

class ColliderSphere : public Collider
{
public:
	ColliderSphere();

private:
	Sphere sphere;
};

class ColliderCapsule : public Collider
{
public:
	ColliderCapsule();

private:
	Capsule capsule;
};

#endif // !COLLIDER_H