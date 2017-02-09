#ifndef MODULECAMERA_H
#define MODULECAMERA_H

#include "Module.h"
#include "Math.h"

#define MODULE_CAMERA "ModuleCamera"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void SetFOV(float fov);
	void SetAspectRatio(float aspect_ratio);
	void SetPlaneDistances(float nearPlaneDistance, float farPlaneDistance);
	void SetPosition(const float3& position);
	void SetOrientation(const float3& rotation);

	void LookAt(const float3& position);

	float* GetProjectionMatrix() const;
	float* GetViewMatrix() const;

private:
	float translation_speed = 1.0f;
	float rotation_speed = 40.0f;
	Frustum* frustum;
};

#endif // !MODULECAMERA_H
