#ifndef COMPONENTCAMERA_H
#define COMPONENTCAMERA_H

#include "Component.h"
#include "Math.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* parent = nullptr);
	~ComponentCamera();

	void SetFOV(float fov);
	void SetAspectRatio(float aspect_ratio);
	void SetPlaneDistances(float nearPlaneDistance, float farPlaneDistance);
	void SetPosition(const float3& position);

	void LookAt(const float3& position);

	float* GetProjectionMatrix() const;
	float* GetViewMatrix() const;

public:
	Frustum* frustum;
};

#endif // !COMPONENTCAMERA_H
