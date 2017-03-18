#ifndef COMPONENTTRANSFORM_H
#define COMPONENTTRANSFORM_H

#include "Component.h"
#include "Math.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* parent);
	~ComponentTransform();

	const float3& GetPosition() const { return position; }
	const float4x4& GetTransform() const { return float4x4::FromTRS(position, rotation, scale); }

	void Load(const float3& position, const float3& scale, const Quat& rotation);

	bool OnUpdate();
	bool OnDraw() const;
	bool OnEditor(int selection_mask, int id);

	float4x4 GetTransformMatrix();

public:
	float3 position = float3::zero;
	float3 scale = float3::one;
	Quat rotation = Quat::identity;
};

#endif
