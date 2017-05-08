#include "ModulePhysics.h"
#include "Physics.h"

ModulePhysics::ModulePhysics()
{
}

ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Init()
{
	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	
	world->setGravity(btVector3(0.0f, -9.8f, 0.0f));

	return true;
}

bool ModulePhysics::Start()
{
	return true;
}

bool ModulePhysics::CleanUp()
{
	return true;
}

update_status ModulePhysics::PreUpdate(float dt)
{
	world->stepSimulation(dt, 15);

	return UPDATE_CONTINUE;
}

update_status ModulePhysics::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModulePhysics::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}