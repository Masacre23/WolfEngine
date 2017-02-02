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
	void SetPlaneDistances();
	void SetPosition();
	void SetOrientation();

	void LookAt(float3 position);

	float4x4 GetProjectionMatrix();
	float4x4 GetViewMatrix();

private:

};

#endif // !MODULECAMERA_H
