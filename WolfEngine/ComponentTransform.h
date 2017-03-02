#ifndef COMPONENTTRANSFORM_H
#define COMPONENTTRANSFORM_H

#include "Component.h"
#include "Math.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(float3 position, float3 scale, Quat rotation);
	~ComponentTransform();

	bool OnUpdate();
	bool OnEditor();

private:
	float3 position;
	float3 scale;
	Quat rotation;
};

#endif
