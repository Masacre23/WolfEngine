#include "ModulePhysics.h"
#include "ComponentRigidBody.h"
#include "Math.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

#ifdef _DEBUG
#pragma comment (lib, "Bullet/libx86/BulletDynamics_debug.lib")
#pragma comment (lib, "Bullet/libx86/BulletCollision_debug.lib")
#pragma comment (lib, "Bullet/libx86/LinearMath_debug.lib")
#else
#pragma comment (lib, "Bullet/libx86/BulletDynamics.lib")
#pragma comment (lib, "Bullet/libx86/BulletCollision.lib")
#pragma comment (lib, "Bullet/libx86/LinearMath.lib")
#endif

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

	for (std::list<btCollisionShape*>::iterator it = shapes.begin(); it != shapes.end(); ++it)
		RELEASE(*it);
	shapes.clear();

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

btRigidBody* ModulePhysics::AddRigidBody(ComponentRigidBody* component, const float3& scaling)
{
	btRigidBody* ret = nullptr;
	
	float mass = 0.0f;
	if (component->GetMotionType() == ComponentRigidBody::MotionType::DYNAMIC)
		mass = component->GetMass();

	btCollisionShape* collision_shape = CreateCollisionShape(component->GetCollider());
	SetCollisionShapeScale(collision_shape, scaling);
	shapes.push_back(collision_shape);
	
	btVector3 local_inertia(0.0f, 0.0f, 0.0f);
	if (mass != 0.0f)
		collision_shape->calculateLocalInertia(mass, local_inertia);

	btRigidBody::btRigidBodyConstructionInfo rigidbody_info(mass, component, collision_shape, local_inertia);
	ret = new btRigidBody(rigidbody_info);
	world->addRigidBody(ret);

	return ret;
}

void ModulePhysics::DeleteRigidBody(btRigidBody* rigid_body, btCollisionShape* collision_shape)
{
	if (rigid_body != nullptr)
	{
		world->removeRigidBody(rigid_body);
		if (collision_shape != nullptr)
		{
			DeleteCollisionShape(collision_shape);
		}
	}	
}

btCollisionShape* ModulePhysics::CreateCollisionShape(Collider* collider)
{
	static_assert(Collider::Type::UNKNOWN == 3, "Update collision shape factory code");

	btCollisionShape* ret = nullptr;

	switch (collider->GetType())
	{
	case Collider::Type::BOX:
		ret = new btBoxShape(((ColliderBox*)collider)->GetBox().r);
		break;
	case Collider::Type::SPHERE:
		ret = new btSphereShape(((ColliderSphere*)collider)->GetSphere().r);
		break;
	case Collider::Type::CAPSULE:
		ret = new btCapsuleShape(((ColliderCapsule*)collider)->GetCapsule().r, ((ColliderCapsule*)collider)->GetCapsule().Height());
		break;
	}

	return ret;
}

void ModulePhysics::DeleteCollisionShape(btCollisionShape* collision_shape)
{
	for (std::list<btCollisionShape*>::iterator it = shapes.begin(); it != shapes.end(); ++it)
	{
		if (*it == collision_shape)
		{
			RELEASE(*it);
			shapes.erase(it);
			break;
		}
	}
}

btCollisionShape* ModulePhysics::GetCollisionShape(btRigidBody* rigid_body)
{
	btCollisionShape* ret;

	if (rigid_body != nullptr)
		ret = rigid_body->getCollisionShape();

	return ret;
}

void ModulePhysics::SetCollisionShapeScale(btCollisionShape* collision_shape, const float3& scaling)
{
	if (collision_shape != nullptr)
	{
		collision_shape->setLocalScaling(scaling);
	}
}

const float3& ModulePhysics::GetCollisionShapeScale(btCollisionShape* collision_shape) const
{
	if (collision_shape != nullptr)
		return collision_shape->getLocalScaling();

	return float3::one;
}

void ModulePhysics::DrawDebug() const
{
	world->debugDrawWorld();
}