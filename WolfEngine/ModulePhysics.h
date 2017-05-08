#ifndef MODULEPHYSICS_H
#define MODULEPHYSICS_H

#include "Module.h"

#define MODULE_PHYSICS "ModulePhysics"
#define PHYSICS_SECTION "Config.Modules.Physics"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

class ModulePhysics
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

private:
	btDefaultCollisionConfiguration* collision_conf = nullptr;
	btCollisionDispatcher* dispatcher = nullptr;
	btBroadphaseInterface* broad_phase = nullptr;
	btSequentialImpulseConstraintSolver* solver = nullptr;
	btDiscreteDynamicsWorld* world = nullptr;
};

#endif // !MODULEPHYSICS_H