#include "Application.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "Color.h"

void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	App->renderer->debug_drawer->DrawLine((float3)from, (float3)to, Color(color.getX(), color.getY(), color.getZ()));
}

void PhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	App->renderer->debug_drawer->DrawPoint((float3)PointOnB, Color(color.getX(), color.getY(), color.getZ()));
}

void PhysicsDebugDrawer::reportErrorWarning(const char* warningString)
{
	APPLOG("Bullet physics warning: %s", warningString);
}

void PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	APPLOG("Bullet draw text: %s", textString);
}

void PhysicsDebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes)debugMode;
}

int	PhysicsDebugDrawer::getDebugMode() const
{
	return mode;
}