#ifndef COMPONENTCAMERA_H
#define COMPONENTCAMERA_H

#include "Component.h"
#include "Math.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* parent = nullptr);
	~ComponentCamera();

	bool OnUpdate();
	bool OnDraw() const;
	bool OnEditor();

	void SetFOV(float fov);
	void SetAspectRatio(float aspect_ratio);
	void SetPlaneDistances(float nearPlaneDistance, float farPlaneDistance);
	void SetPosition(const float3& position);
	void SetOrientation(const Quat& rotation);

	void LookAt(const float3& position);

	bool IsInsideFrustum(const AABB& box) const;

	float* GetProjectionMatrix() const;
	float* GetViewMatrix() const;

public:
	Frustum* frustum;
	bool frustum_culling = true;

};

#endif // !COMPONENTCAMERA_H
