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
	const float4x4& GetLocalTransformMatrix() const { return local_transform; }
	void CalculateLocalTransformMatrixNoRotation(float4x4& local_transform) const;

	void Load(const float3& position, const float3& scale, const Quat& rotation);
	void Load(const float3& position, const Quat& rotation);

	bool OnUpdate();
	bool OnDraw() const;
	bool OnEditor();

private:
	void RecalculateLocalTransform();

private:
	float3 position = float3::zero;
	float3 scale = float3::one;
	Quat rotation = Quat::identity;

	float4x4 local_transform = float4x4::identity;
};

#endif
