#include "Application.h"
#include "ModuleRender.h"
#include "Collider.h"
#include "Color.h"
#include "Interface.h"

Collider::Collider(Type type) : type(type)
{
}

Collider::~Collider()
{
}

ColliderBox::ColliderBox() : Collider(Type::BOX)
{
	box.pos = float3::zero;
	box.r = float3::one;
	box.axis[0] = float3::unitX;
	box.axis[1] = float3::unitY;
	box.axis[2] = float3::unitZ;

	transform = float4x4::FromTRS(box.pos, Quat::identity, float3::one);
}

void ColliderBox::OnEditor()
{
	ImGui::TextWrapped("Box Collider");

	//ImGui::DragFloat3("Position", (float*)&box.pos, 0.1f);
	float3 size = 2.0f * box.r;
	ImGui::DragFloat3("Size", (float*)&size, 0.1f);
	box.r = size * 0.5f;
}

void ColliderBox::OnDebugDraw()
{
	App->renderer->debug_drawer->DrawBoundingBox(box, Colors::Green);
}

ColliderSphere::ColliderSphere() : Collider(Type::SPHERE)
{
	sphere.r = 1.0;
	sphere.pos = float3::zero;

	transform = float4x4::FromTRS(sphere.pos, Quat::identity, float3::one);
}

ColliderCapsule::ColliderCapsule() : Collider(Type::CAPSULE)
{
}
