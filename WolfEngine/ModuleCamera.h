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
	bool CleanUp();

	void SetFOV();
	void SetAspectRatio();
	void SetPlaneDistances();
	void SetPosition();
	void SetOrientation();

	void LookAt(float3 position);

	float* GetProjectionMatrix();
	float* GetViewMatrix();

private:
	Frustum* frustum;

};

#endif // !MODULECAMERA_H
