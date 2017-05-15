#ifndef COMPONENTRIGIDBODY_H
#define COMPONENTRIGIDBODY_H

#include "Component.h"
#include "Collider.h"

class btRigidBody;

class ComponentRigidBody : public Component 
{
public:
	enum MotionType
	{
		STATIC,
		KINEMATIC,
		DYNAMIC
	};

public:
	ComponentRigidBody(GameObject * parent);
	~ComponentRigidBody();
	
	bool OnDebugDraw() const;
	bool OnEditor();

	void LoadRigidBody(Collider::Type collider_type, float mass = 1.0f, MotionType motion_type = DYNAMIC);

private:
	Collider* CreateCollider(Collider::Type type);

private:
	btRigidBody* rigid_body = nullptr;

	Collider* collider = nullptr;
	MotionType motion_type = STATIC;
	float mass = 1.0f;
};

#endif // !COMPONENTRIGIDBODY_H