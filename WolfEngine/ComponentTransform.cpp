#include "ComponentTransform.h"
#include "OpenGL.h"

ComponentTransform::ComponentTransform(GameObject* parent) : Component(Component::Type::TRANSFORM, parent)
{
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Load(const float3& position, const float3& scale, const Quat& rotation)
{
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
}

bool ComponentTransform::OnUpdate()
{
	return true;
}

bool ComponentTransform::OnDraw() const
{
	float* transform = (float*)float4x4::FromTRS(position, rotation, scale).v;
	glMultMatrixf(transform);

	return true;
}

bool ComponentTransform::OnEditor()
{
//	ImGui::Text("Yolo");
	return false;
}
