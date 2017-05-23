#include "Application.h"
#include "ModuleRender.h"
#include "ComponentRigidBody.h"
#include "Collider.h"
#include "Color.h"
#include "Interface.h"
#include "GameObject.h"
#include "ModuleTimeController.h"

Collider::Collider(Type type, ComponentRigidBody* parent) : type(type), parent(parent)
{
}

Collider::~Collider()
{
}

void Collider::RecalculateLocalTransform(const float3& position)
{
	transform = float4x4::identity;
	transform.SetTranslatePart(position);
}

ColliderBox::ColliderBox(ComponentRigidBody* parent) : Collider(Type::BOX, parent)
{
	box.pos = float3::zero;
	box.r = 0.5f * float3::one;
	box.axis[0] = float3::unitX;
	box.axis[1] = float3::unitY;
	box.axis[2] = float3::unitZ;

	RecalculateLocalTransform(box.pos);
}

void ColliderBox::OnEditor()
{
	ImGui::DragFloat3("Position", (float*)&box.pos, 0.1f);
	float3 size = 2.0f * box.r;
	ImGui::DragFloat3("Size", (float*)&size, 0.1f);
	box.r = size * 0.5f;

	RecalculateLocalTransform(box.pos);
}

void ColliderBox::OnDebugDraw()
{
	if (App->time_controller->IsStopped())
		App->renderer->debug_drawer->DrawBoundingBox(box, Colors::Green);
}

void ColliderBox::SetOnVertices(float3* vertices, unsigned num_vertices)
{
	AABB aabb;
	aabb.SetNegativeInfinity();
	aabb.Enclose(vertices, num_vertices);
	
	box.SetNegativeInfinity();
	box.SetFrom(aabb, transform);
	RecalculateLocalTransform(box.pos);
}

ColliderSphere::ColliderSphere(ComponentRigidBody* parent) : Collider(Type::SPHERE, parent)
{
	sphere.r = 0.1;
	sphere.pos = float3::zero;

	RecalculateLocalTransform(sphere.pos);
}

void ColliderSphere::OnEditor()
{
	ImGui::DragFloat3("Position", (float*)&sphere.pos, 0.1f);
	ImGui::DragFloat("Radius", (float*)&sphere.r, 0.1f);

	RecalculateLocalTransform(sphere.pos);
}

void ColliderSphere::OnDebugDraw()
{
	if (App->time_controller->IsStopped())
		App->renderer->debug_drawer->DrawSphere(sphere, Colors::Green);
}	

void ColliderSphere::SetOnVertices(float3* vertices, unsigned num_vertices)
{
	AABB aabb;
	aabb.SetNegativeInfinity();
	aabb.Enclose(vertices, num_vertices);

	OBB obb;
	obb.SetNegativeInfinity();
	obb.SetFrom(aabb, transform);

	sphere = obb.MaximalContainedSphere();
	RecalculateLocalTransform(sphere.pos);
}

ColliderCapsule::ColliderCapsule(ComponentRigidBody* parent) : Collider(Type::CAPSULE, parent)
{
}
