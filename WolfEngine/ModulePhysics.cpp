#include "ModulePhysics.h"
#include "Physics.h"

ModulePhysics::ModulePhysics() : Module(MODULE_PHYSICS)
{
}

ModulePhysics::~ModulePhysics()
{
	RELEASE(debug_drawer);
	RELEASE(solver);
	RELEASE(broad_phase);
	RELEASE(dispatcher);
	RELEASE(collision_conf);
}

bool ModulePhysics::Init()
{
	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	debug_drawer = new PhysicsDebugDrawer();

	return true;
}

bool ModulePhysics::Start()
{
	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	world->setDebugDrawer(debug_drawer);
	world->setGravity(gravity);

	return true;
}

bool ModulePhysics::CleanUp()
{
	APPLOG("Cleaning physic world");

	RELEASE(world);	

	return true;
}

update_status ModulePhysics::PreUpdate(float dt)
{
	BROFILER_CATEGORY("ModulePhysics-PreUpdate", Profiler::Color::Blue);

	world->stepSimulation(dt, 15);

	return UPDATE_CONTINUE;
}

update_status ModulePhysics::Update(float dt)
{
	BROFILER_CATEGORY("ModulePhysics-Update", Profiler::Color::Red);

	return UPDATE_CONTINUE;
}

update_status ModulePhysics::PostUpdate(float dt)
{
	BROFILER_CATEGORY("ModulePhysics-PostUpdate", Profiler::Color::Green);

	return UPDATE_CONTINUE;
}

void ModulePhysics::DrawDebug() const
{
	world->debugDrawWorld();
}