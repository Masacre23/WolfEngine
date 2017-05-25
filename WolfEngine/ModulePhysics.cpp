#include "Application.h"
#include "ModulePhysics.h"
#include "ModuleLevel.h"
#include "GameObject.h"
#include "ComponentRigidBody.h"
#include "ComponentMesh.h"
#include "Collider.h"
#include "Math.h"
#include "Color.h"
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

	debug_drawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

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

	for (std::list<btTriangleMesh*>::iterator it = triangle_meshes.begin(); it != triangle_meshes.end(); ++it)
		RELEASE(*it);
	triangle_meshes.clear();

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
	collision_shape->setLocalScaling(scaling);
	shapes.push_back(collision_shape);
	
	btVector3 local_inertia(0.0f, 0.0f, 0.0f);
	if (mass != 0.0f)
		collision_shape->calculateLocalInertia(mass, local_inertia);

	btRigidBody::btRigidBodyConstructionInfo rigidbody_info(mass, component, collision_shape, local_inertia);
	ret = new btRigidBody(rigidbody_info);
	world->addRigidBody(ret);

	collision_shape->getShapeType();

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
	static_assert(Collider::Type::UNKNOWN == 4, "Update collision shape factory code");

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
		ret = new btCapsuleShape(((ColliderCapsule*)collider)->GetCapsule().r, ((ColliderCapsule*)collider)->GetCapsule().LineLength());
		break;
	case Collider::Type::MESH:
		btTriangleMesh* tri_mesh = CreateTriangleMesh(collider);
		ret = new btBvhTriangleMeshShape(tri_mesh, false);
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

btTriangleMesh* ModulePhysics::CreateTriangleMesh(Collider* collider)
{
	btTriangleMesh* ret = new btTriangleMesh();

	std::vector<ComponentMesh*> meshes = collider->GetMeshes();
	if (meshes.size() > 0)
	{
		for (std::vector<ComponentMesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		{
			unsigned num_vertices = (*it)->GetNumVertices();
			unsigned num_indices = (*it)->GetNumIndices();
			unsigned num_triangles = num_indices / 3;

			const float3* vertex = (*it)->GetVertices();
			const unsigned* indices = (*it)->GetIndices();

			for (int i = 0; i < num_triangles; i++)
			{
				int row0 = indices[3 * i];
				if (row0 > num_vertices)
					APPLOG("ERRORRRR");
				int row1 = indices[3 * i + 1];
				if (row1 > num_vertices)
					APPLOG("ERRORRRR");
				int row2 = indices[3 * i + 2];
				if (row2 > num_vertices)
					APPLOG("ERRORRRR");
				ret->addTriangle(vertex[row0], vertex[row1], vertex[row2]);
			}
		}
		triangle_meshes.push_back(ret);
	}

	return ret;
}

btCollisionShape* ModulePhysics::GetCollisionShape(btRigidBody* rigid_body)
{
	btCollisionShape* ret;

	if (rigid_body != nullptr)
		ret = rigid_body->getCollisionShape();

	return ret;
}

const float3& ModulePhysics::GetCollisionShapeScale(btCollisionShape* collision_shape) const
{
	if (collision_shape != nullptr)
		return collision_shape->getLocalScaling();

	return float3::one;
}

void ModulePhysics::DrawDebug() const
{
	BROFILER_CATEGORY("ModulePhysics-DebugDraw", Profiler::Color::GreenYellow);

	//With a lot of meshes as shapes, the world debug takes forever (~200 ms)
	//Maybe can be improved by overriding some debugDrawWorld functions
	//In the end, is better to just debugDraw the selected item

	//world->debugDrawWorld();

	//Maybe better to only show shapes on the selected element

	GameObject* selected = App->level->GetSelectedGameObject();
	if (selected != nullptr)
	{
		ComponentRigidBody* rigid_body = (ComponentRigidBody*)selected->GetComponent(Component::Type::RIGIDBODY);
		if (rigid_body != nullptr)
		{
			Collider* collider = rigid_body->GetCollider();
			if (collider != nullptr)
			{
				btCollisionShape* shape = collider->GetCollisionShape();
				if (shape != nullptr)
				{
					btTransform world_transform;
					rigid_body->getWorldTransform(world_transform);
					world->debugDrawObject(world_transform, shape, btVector3(0.0f, 1.0f, 0.0f));
				}
			}
		}
	}
	
}