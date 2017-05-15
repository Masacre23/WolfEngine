#ifndef MODULEPHYSICS_H
#define MODULEPHYSICS_H

#include "Module.h"
#include "Bullet/include/LinearMath/btIDebugDraw.h"
#include <vector>

#define MODULE_PHYSICS "ModulePhysics"
#define PHYSICS_SECTION "Config.Modules.Physics"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class PhysicsDebugDrawer;

class btRigidBody;
class ComponentRigidBody;
class btCollisionShape;
class Collider;

class ModulePhysics : public Module
{
public:
	ModulePhysics();
	~ModulePhysics();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void DrawDebug() const;

	btRigidBody* AddRigidBody(ComponentRigidBody* component);
	void DeleteRigidBody(btRigidBody* rigid_body);

	btCollisionShape* CreateCollisionShape(Collider* collider);
	void DeleteCollisionShape(btCollisionShape* collision_shape);

private:
	btDefaultCollisionConfiguration* collision_conf = nullptr;
	btCollisionDispatcher* dispatcher = nullptr;
	btBroadphaseInterface* broad_phase = nullptr;
	btSequentialImpulseConstraintSolver* solver = nullptr;
	btDiscreteDynamicsWorld* world = nullptr;
	PhysicsDebugDrawer* debug_drawer = nullptr;

	btVector3 gravity = btVector3(0.0f, -9.8f, 0.0f);

	std::vector<btCollisionShape*> shapes;
};

class PhysicsDebugDrawer : public btIDebugDraw
{
public:
	PhysicsDebugDrawer() {}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

	void setDebugMode(int debugMode);
	int getDebugMode() const;

	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);

public:
	DebugDrawModes mode;
};

#endif // !MODULEPHYSICS_H