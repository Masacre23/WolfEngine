#include "ComponentTransform.h"

ComponentTransform::ComponentTransform() : Component(TRANSFORM)
{
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Load(float3 position, float3 scale, Quat rotation)
{
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
}

bool ComponentTransform::OnUpdate()
{
	glTranslatef(position.x, position.y, position.z);
	glScalef(scale.x, scale.y, scale.z);
	glRotatef(rotation.Angle(), rotation.Axis().x, rotation.Axis().y, rotation.Axis().z);

	return true;
}

bool ComponentTransform::OnEditor()
{
	ImGui::Text("Yolo");
	return false;
}
