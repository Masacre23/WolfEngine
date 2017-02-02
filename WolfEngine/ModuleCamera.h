#ifndef MODULECAMERA_H
#define MODULECAMERA_H

#include "Module.h"
#include "MathGeoLib/src/Math/float3.h"

#define MODULE_CAMERA "ModuleCamera"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	void SetFOV(float fov);
	void SetAspectRatio(float r);
	void SetPlaneDistances(float nearPlaneDistance, float farPlaneDistance);
	void SetPosition(float3 position);
	void SetOrientation(float3 rotation);

	void LookAt(float3 position);

	float4x4 GetProjectionMatrix();
	float4x4 GetViewMatrix();

	void WindowResize(int width, int height);

private:
	void SetFOH(float fov, float r);

private:
	Frustum* frustum;
};

#endif // !MODULECAMERA_H
