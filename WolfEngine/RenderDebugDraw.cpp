#include "ModuleRender.h"
#include "Color.h"
#include "OpenGL.h"

RenderDebugDrawer::RenderDebugDrawer()
{
}

RenderDebugDrawer::~RenderDebugDrawer()
{
}

void RenderDebugDrawer::PreDebugDraw()
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
}

void RenderDebugDrawer::PostDebugDraw()
{
	glLineWidth(1.0f);
	glPointSize(1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}

void RenderDebugDrawer::SetColor(const Color& color)
{
	glColor3f(color.r, color.g, color.b);
}

void RenderDebugDrawer::DrawAxis()
{
	float axis_length = 1.5f;

	glDepthRange(0.0, 0.01);

	glLineWidth(2.0f);

	glColor3f(Colors::Red.r, Colors::Red.g, Colors::Red.b);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(axis_length, 0.0, 0.0);
	glEnd();

	glColor3f(Colors::Green.r, Colors::Green.g, Colors::Green.b);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, axis_length, 0.0);
	glEnd();

	glColor3f(Colors::Blue.r, Colors::Blue.g, Colors::Blue.b);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, axis_length);
	glEnd();

	glDepthRange(0.01, 1.0);

}

void RenderDebugDrawer::DrawBoundingBox(const AABB& bbox, const Color& color)
{
	float3 corners[8];
	bbox.GetCornerPoints(corners);

	DrawParallepiped(corners, color);
}

void RenderDebugDrawer::DrawBoundingBox(const OBB& bbox, const Color& color)
{
	float3 corners[8];
	bbox.GetCornerPoints(corners);

	DrawParallepiped(corners, color);
}

void RenderDebugDrawer::DrawFrustum(const Frustum& frustum, const Color& color)
{
	float3 corners[8];
	frustum.GetCornerPoints(corners);

	DrawParallepiped(corners, color);
}

void RenderDebugDrawer::DrawLine(const float3& from, const float3& to, const Color& color)
{
	glColor3f(color.r, color.g, color.b);

	glLineWidth(2.0f);

	glBegin(GL_LINES);
	
	glVertex3fv((GLfloat*)&from);
	glVertex3fv((GLfloat*)&to);

	glEnd();
}

void RenderDebugDrawer::DrawPoint(const float3& point, const Color& color)
{
	glColor3f(color.r, color.g, color.b);

	glPointSize(2.0f);

	glBegin(GL_POINTS);

	glVertex3fv((GLfloat*)&point);

	glEnd();
}

void RenderDebugDrawer::DrawParallepiped(const float3* corners, const Color& color)
{
	glColor3f(color.r, color.g, color.b);

	glLineWidth(1.0f);
	glBegin(GL_LINES);

	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[1]);

	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[2]);

	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[4]);

	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[1]);

	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[4]);

	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[7]);

	glVertex3fv((GLfloat*)&corners[3]);
	glVertex3fv((GLfloat*)&corners[7]);

	glVertex3fv((GLfloat*)&corners[3]);
	glVertex3fv((GLfloat*)&corners[1]);

	glVertex3fv((GLfloat*)&corners[3]);
	glVertex3fv((GLfloat*)&corners[2]);

	glVertex3fv((GLfloat*)&corners[6]);
	glVertex3fv((GLfloat*)&corners[7]);

	glVertex3fv((GLfloat*)&corners[6]);
	glVertex3fv((GLfloat*)&corners[4]);

	glVertex3fv((GLfloat*)&corners[6]);
	glVertex3fv((GLfloat*)&corners[2]);

	glEnd();
}