#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(float3 position, float3 scale, Quat rotation) : position(position), scale(scale), rotation(rotation), Component(TRANSFORM)
{
}

ComponentTransform::~ComponentTransform()
{
}

bool ComponentTransform::OnUpdate()
{
	return true;
}

bool ComponentTransform::OnEditor()
{
	return false;
}
