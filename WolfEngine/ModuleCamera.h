#ifndef MODULECAMERA_H
#define MODULECAMERA_H

#include "Module.h"
#include "Math.h"

#define MODULE_CAMERA "ModuleCamera"
#define CAMERA_SECTION "Config.Modules.EditorCamera"

class ComponentCamera;

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

	void LookAt(const float3& position);

	float* GetProjectionMatrix() const;
	float* GetViewMatrix() const;

private:
	float speed_rotation = 0.5f;
	float speed_translation = 1.0f;
	float extra_speed_zoom = 10.0f;
	
	ComponentCamera* editor_camera = nullptr;
	ComponentCamera* main_camera = nullptr;
};

#endif // !MODULECAMERA_H
