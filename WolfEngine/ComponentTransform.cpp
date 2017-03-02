#include "ComponentTransform.h"
#include "OpenGL.h"

ComponentTransform::ComponentTransform(float3 position, float3 scale, Quat rotation) : position(position), scale(scale), rotation(rotation), Component(TRANSFORM)
{
}

ComponentTransform::~ComponentTransform()
{
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
	return false;
}
