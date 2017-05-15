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

	const OBB& GetBox() const { return box; }

private:
	OBB box;
};

class ColliderSphere : public Collider
{
public:
	ColliderSphere();

	const Sphere& GetSphere() const { return sphere; }

private:
	Sphere sphere;
};

class ColliderCapsule : public Collider
{
public:
	ColliderCapsule();

	const Capsule& GetCapsule() const { return capsule; }

private:
	Capsule capsule;
};

#endif // !COLLIDER_H