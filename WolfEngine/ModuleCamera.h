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
	void SetAspectRatio(float r);
	void SetPlaneDistances(float nearPlaneDistance, float farPlaneDistance);
	void SetPosition(float3 position);
	void SetOrientation(float3 rotation);

	void LookAt(float3 position);

	float* GetProjectionMatrix();
	float* GetViewMatrix();

	void WindowResize(int width, int height);

private:
	void SetFOH(float fov, float r);

private:
	Frustum* frustum;
};

#endif // !MODULECAMERA_H
