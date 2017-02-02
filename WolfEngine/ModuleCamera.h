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

	void SetFOV();
	void SetAspectRatio();
	void SetPlaneDistances(float nearPlaneDistance, float farPlaneDistance);
	void SetPosition(float3 position);
	void SetOrientation(float3 rotation);

	void LookAt(float3 position);

	float4x4 GetProjectionMatrix();
	float4x4 GetViewMatrix();

private:
	Frustum* frustum;
};

#endif // !MODULECAMERA_H
