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

	bool Init();
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
	float3 GetPosition() const;

	bool InsideCulling(const AABB& box) const;

	void SetupFrustum(ComponentCamera* camera);

public:
	ComponentCamera* editor_camera = nullptr;
	ComponentCamera* frustum_camera = nullptr;

private:
	float NEARPLANE = 0.1f;
	float FARPLANE = 5000.0f;
	float VERTICALFOV = 59.0f;
	float ASPECTRATIO = 1.5f;

	float SPEED_ROTATION = 0.5f;
	float SPEED_TRANSLATION = 1.0f;
	float SPEED_ZOOM = 10.0f;

};

#endif // !MODULECAMERA_H
