#ifndef COLLIDER_H
#define COLLIDER_H

#include "Math.h"
#include <vector>

class ComponentRigidBody;
class ComponentMesh;
class btCollisionShape;

class Collider
{
public:
	enum Type
	{
		BOX = 0,
		SPHERE,
		CAPSULE,
		MESH,
		UNKNOWN
	};

public:
	Collider(Type type, ComponentRigidBody* parent);
	~Collider();

	virtual void OnEditor() {}
	virtual void OnDebugDraw() {}

	virtual const float3& GetPosition() const { return float3::zero; }

	void SetShapeOnMeshes();
	void SetMeshes(std::vector<ComponentMesh*>& meshes);

	void SetCollisionShape(btCollisionShape* collision_shape) { this->collision_shape = collision_shape; }
	btCollisionShape* GetCollisionShape() const { return collision_shape; }
	void UnsetCollisionShape() { collision_shape = nullptr; }
	virtual void OnStop() { collision_shape = nullptr; }

	Type GetType() { return type; }
	const std::vector<ComponentMesh*>& GetMeshes() const { return meshes; }

	const float4x4& GetLocalTransform() const { return transform; }

protected:
	void RecalculateLocalTransform(const float3& position = float3::zero);

private:
	virtual void SetOnVertices(float3* vertices, unsigned num_vertices) {}

protected:
	ComponentRigidBody* parent;
	float4x4 transform = float4x4::identity;
	Type type;
	btCollisionShape* collision_shape = nullptr;
	std::vector<ComponentMesh*> meshes;
};

class ColliderBox : public Collider
{
public:
	ColliderBox(ComponentRigidBody* parent);

	void OnEditor();
	void OnDebugDraw();
	void SetOnVertices(float3* vertices, unsigned num_vertices);
	const float3& GetPosition() const { return box.pos; }

	const OBB& GetBox() const { return box; }

private:
	OBB box;
};

class ColliderSphere : public Collider
{
public:
	ColliderSphere(ComponentRigidBody* parent);

	void OnEditor();
	void OnDebugDraw();
	void SetOnVertices(float3* vertices, unsigned num_vertices);
	const float3& GetPosition() const { return sphere.pos; }

	const Sphere& GetSphere() const { return sphere; }

private:
	Sphere sphere;
};

class ColliderCapsule : public Collider
{
public:
	ColliderCapsule(ComponentRigidBody* parent);

	void OnEditor();
	void OnDebugDraw();
	void SetOnVertices(float3* vertices, unsigned num_vertices);
	const float3& GetPosition() const { return capsule.l.CenterPoint(); }

	const Capsule& GetCapsule() const { return capsule; }

private:
	Capsule capsule;
	float total_height = 0.0f;
};

class ColliderMesh : public Collider
{
public:
	ColliderMesh(ComponentRigidBody* parent);

	void OnEditor();
	void OnStop() {}

private:

};

#endif // !COLLIDER_H