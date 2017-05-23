#ifndef COMPONENTRIGIDBODY_H
#define COMPONENTRIGIDBODY_H

#include "Component.h"
#include "Collider.h"
#include "Bullet/include/LinearMath/btMotionState.h"
#include "Bullet/include/LinearMath/btTransform.h"

class btRigidBody;

class ComponentRigidBody : public Component, public btMotionState
{
public:
	enum MotionType
	{
		STATIC = 0,
		KINEMATIC,
		DYNAMIC
	};

public:
	ComponentRigidBody(GameObject * parent);
	~ComponentRigidBody();
	
	void OnDebugDraw() const;
	bool OnEditor();

	void OnPlay();
	void OnStop();

	void LoadRigidBody(Collider::Type collider_type, float mass = 0.0f, MotionType motion_type = MotionType::STATIC);
	void LoadRigidBody(float mass = 0.0f, MotionType motion_type = MotionType::STATIC);
	void LoadCollider(Collider::Type collider_type);
	void LoadCollider(Collider::Type collider_type, float3* vertices, unsigned num_vertices);

	Collider* GetCollider() const { return collider; }
	MotionType GetMotionType() const { return motion_type; }
	float GetMass() const { return mass; }

	void getWorldTransform(btTransform& worldTrans) const override;
	void setWorldTransform(const btTransform& worldTrans) override;

private:
	Collider* CreateCollider(Collider::Type type);

private:
	MotionType motion_type = STATIC;
	Collider* collider = nullptr;
	float mass = 1.0f;

	btRigidBody* rigid_body = nullptr;
};

#endif // !COMPONENTRIGIDBODY_H