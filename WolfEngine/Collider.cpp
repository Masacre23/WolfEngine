#include "Application.h"
#include "ModuleRender.h"
#include "ComponentRigidBody.h"
#include "ComponentMesh.h"
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

void Collider::SetShapeOnMeshes()
{
	unsigned numvert_total = 0;
	for (std::vector<ComponentMesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		numvert_total += (*it)->GetNumVertices();

	float3* vert_total = new float3[numvert_total];
	unsigned offset = 0;
	for (std::vector<ComponentMesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		unsigned num_vertices = (*it)->GetNumVertices();
		float3* vertex_pointer = &(vert_total[offset]);
		memcpy(vertex_pointer, (*it)->GetVertices(), num_vertices * sizeof(float3));
		offset += num_vertices;
	}

	SetOnVertices(vert_total, numvert_total);

	RELEASE_ARRAY(vert_total);
}

void Collider::SetMeshes(std::vector<ComponentMesh*>& meshes)
{
	this->meshes = meshes;
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
	ImGui::DragFloat3("Center", (float*)&box.pos, 0.1f);
	float3 size = 2.0f * box.r;
	ImGui::DragFloat3("Size", (float*)&size, 0.1f);
	box.r = size * 0.5f;

	RecalculateLocalTransform(box.pos);
}

void ColliderBox::OnDebugDraw()
{
	App->renderer->debug_drawer->DrawBox(box, Colors::Green, transform);
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
	ImGui::DragFloat3("Center", (float*)&sphere.pos, 0.1f);
	ImGui::DragFloat("Radius", (float*)&sphere.r, 0.1f);

	RecalculateLocalTransform(sphere.pos);
}

void ColliderSphere::OnDebugDraw()
{
	App->renderer->debug_drawer->DrawSphere(sphere, Colors::Green, transform);
}	

void ColliderSphere::SetOnVertices(float3* vertices, unsigned num_vertices)
{
	AABB aabb;
	aabb.SetNegativeInfinity();
	aabb.Enclose(vertices, num_vertices);

	/*LineSegment line;
	line.a = aabb.maxPoint;
	line.b = aabb.minPoint;
	sphere.pos = aabb.CenterPoint();
	sphere.r = line.Length() / 2.0f;*/

	sphere.pos = aabb.CenterPoint();
	sphere.r = 0.0f;
	for (unsigned i = 0; i < num_vertices; i++)
		sphere.ExtendRadiusToContain(vertices[i]);

	RecalculateLocalTransform(sphere.pos);
}

ColliderCapsule::ColliderCapsule(ComponentRigidBody* parent) : Collider(Type::CAPSULE, parent)
{
	capsule.r = 2.0f;
	capsule.l.a.x = 0.0f;
	capsule.l.a.y = 0.5f;
	capsule.l.a.z = 0.0f;
	capsule.l.b.x = 0.0f;
	capsule.l.b.y = -0.5f;
	capsule.l.b.z = 0.0f;

	total_height = capsule.Height();

	RecalculateLocalTransform(capsule.l.CenterPoint());
}

void ColliderCapsule::OnEditor()
{
	float3 center = capsule.l.CenterPoint();
	float radius = capsule.r;
	float height = total_height;

	ImGui::DragFloat3("Center", (float*)&center, 0.1f);

	ImGui::DragFloat("Radius", (float*)&radius, 0.1f);

	ImGui::DragFloat("Height", (float*)&height, 0.1f);

	float segment_height = height - 2.0f * radius;
	if (segment_height < 0.0f)
	{
		if (height != total_height)
			total_height = height;
		height = 2.0f * radius;
		segment_height = 0.0f;	
	}
	else
		total_height = height;

	capsule.r = radius;
	capsule.l.a = center + 0.5f * segment_height * float3::unitY;
	capsule.l.b = center - 0.5f * segment_height * float3::unitY;

	RecalculateLocalTransform(center);
}

void ColliderCapsule::OnDebugDraw()
{
	App->renderer->debug_drawer->DrawCapsule(capsule, Colors::Green, transform);
}

void ColliderCapsule::SetOnVertices(float3* vertices, unsigned num_vertices)
{
	AABB aabb;
	aabb.SetNegativeInfinity();
	aabb.Enclose(vertices, num_vertices);

	float3 center = aabb.CenterPoint();
	capsule.r = (aabb.maxPoint - aabb.FaceCenterPoint(3)).Length();
	capsule.l.a.x = center.x;
	capsule.l.a.y = aabb.maxPoint.y;
	capsule.l.a.z = center.z;
	capsule.l.b.x = center.x;
	capsule.l.b.y = aabb.minPoint.y;
	capsule.l.b.z = center.z;

	total_height = capsule.Height();

	RecalculateLocalTransform(capsule.l.CenterPoint());
}

ColliderMesh::ColliderMesh(ComponentRigidBody* parent) : Collider(Type::MESH, parent)
{
}

void ColliderMesh::OnEditor()
{

}
