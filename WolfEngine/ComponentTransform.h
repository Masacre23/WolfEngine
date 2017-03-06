#ifndef COMPONENTTRANSFORM_H
#define COMPONENTTRANSFORM_H

#include "Component.h"
#include "Math.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* parent);
	~ComponentTransform();

	void Load(const float3& position, const float3& scale, const Quat& rotation);

	bool OnUpdate();
	bool OnDraw() const;
	bool OnEditor();

private:
	float3 position = float3::zero;
	float3 scale = float3::one;
	Quat rotation = Quat::identity;
};

#endif
