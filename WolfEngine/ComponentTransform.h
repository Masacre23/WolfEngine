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
	const Quat& GetRotation() const { return rotation; }
	const float4x4& GetLocalTransformMatrix() const { return local_transform; }
	const float4x4& GetGlobalTransformMatrix() const { return global_transform; }

	const float4x4& UpdateTransform(const float4x4& parent);

	void Load(const float3& position, const float3& scale, const Quat& rotation);
	void Load(const float3& position, const Quat& rotation);

	bool OnUpdate();
	bool OnDraw() const;
	bool OnEditor();

private:
	void RecalculateLocalTransform();
	
public:
	float4x4 local_transform = float4x4::identity;
	float4x4 backup_local_transform = float4x4::identity;
	float4x4 global_transform = float4x4::identity;
	bool transform_change = false;

private:
	float3 position = float3::zero;
	float3 scale = float3::one;
	Quat rotation = Quat::identity;
	float3 rotation_euler = float3::zero;
};

#endif
