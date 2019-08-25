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

void RenderDebugDrawer::DrawAxis(const float4x4& transform)
{
	float axis_length = 1.5f;

	glPushMatrix();
	glMultMatrixf((GLfloat*)transform.Transposed().ptr());

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

	glPopMatrix();
}

void RenderDebugDrawer::DrawBoundingBox(const AABB& bbox, const Color& color, const float4x4& transform)
{
	float3 corners[8];
	bbox.GetCornerPoints(corners);

	glPushMatrix();
	glMultMatrixf((GLfloat*) transform.Transposed().ptr());
	DrawParallepiped(corners, color);
	glPopMatrix();
}

void RenderDebugDrawer::DrawBoundingBox(const OBB& bbox, const Color& color, const float4x4& transform)
{
	float3 corners[8];
	bbox.GetCornerPoints(corners);

	glPushMatrix();
	glMultMatrixf((GLfloat*)transform.Transposed().ptr());
	DrawParallepiped(corners, color);
	glPopMatrix();
}

void RenderDebugDrawer::DrawFrustum(const Frustum& frustum, const Color& color, const float4x4& transform)
{
	float3 corners[8];
	frustum.GetCornerPoints(corners);

	glPushMatrix();
	glMultMatrixf((GLfloat*)transform.Transposed().ptr());
	DrawParallepiped(corners, color);
	glPopMatrix();
}

void RenderDebugDrawer::DrawLine(const float3& from, const float3& to, const Color& color, const float4x4& transform)
{
	glPushMatrix();
	glMultMatrixf((GLfloat*)transform.Transposed().ptr());

	glColor3f(color.r, color.g, color.b);

	glLineWidth(2.0f);

	glBegin(GL_LINES);
	
	glVertex3fv((GLfloat*)&from);
	glVertex3fv((GLfloat*)&to);

	glEnd();

	glPopMatrix();
}

void RenderDebugDrawer::DrawPoint(const float3& point, const Color& color, const float4x4& transform)
{
	glPushMatrix();
	glMultMatrixf((GLfloat*)transform.Transposed().ptr());

	glColor3f(color.r, color.g, color.b);

	glPointSize(2.0f);

	glBegin(GL_POINTS);

	glVertex3fv((GLfloat*)&point);

	glEnd();

	glPopMatrix();
}

void RenderDebugDrawer::DrawBox(const OBB& box, const Color& color, const float4x4& transform)
{
	glPushMatrix();
	glMultMatrixf((GLfloat*)transform.Transposed().ptr());

	glColor3f(color.r, color.g, color.b);
	glLineWidth(1.0f);

	glBegin(GL_LINES);
	
	glVertex3f(box.r.x, box.r.y, box.r.z);
	glVertex3f(box.r.x, box.r.y, - box.r.z);

	glVertex3f(box.r.x, box.r.y, box.r.z);
	glVertex3f(box.r.x, -box.r.y, box.r.z);

	glVertex3f(box.r.x, box.r.y, box.r.z);
	glVertex3f(-box.r.x, box.r.y, box.r.z);

	glVertex3f(-box.r.x, -box.r.y, -box.r.z);
	glVertex3f(-box.r.x, -box.r.y, box.r.z);

	glVertex3f(-box.r.x, -box.r.y, -box.r.z);
	glVertex3f(-box.r.x, box.r.y, -box.r.z);

	glVertex3f(-box.r.x, -box.r.y, -box.r.z);
	glVertex3f(box.r.x, -box.r.y, -box.r.z);

	glVertex3f(box.r.x, - box.r.y, - box.r.z);
	glVertex3f(box.r.x, - box.r.y, box.r.z);

	glVertex3f(box.r.x, -box.r.y, box.r.z);
	glVertex3f(box.r.x, -box.r.y, -box.r.z);

	glVertex3f(box.r.x, -box.r.y, box.r.z);
	glVertex3f(-box.r.x, -box.r.y, box.r.z);

	glVertex3f(-box.r.x, box.r.y, -box.r.z);
	glVertex3f(-box.r.x, box.r.y, box.r.z);

	glVertex3f(-box.r.x, box.r.y, -box.r.z);
	glVertex3f(box.r.x, box.r.y, -box.r.z);

	glVertex3f(box.r.x, box.r.y, -box.r.z);
	glVertex3f(box.r.x, -box.r.y, -box.r.z);

	glVertex3f(-box.r.x, box.r.y, box.r.z);
	glVertex3f(-box.r.x, -box.r.y, box.r.z);

	glEnd();

	glPopMatrix();
}

void RenderDebugDrawer::DrawSphere(const Sphere& sphere, const Color& color, const float4x4& transform)
{
	int num_arc_points = 4;
	float angle_interval = (90 / num_arc_points) * DEG_TO_RAD;

	glPushMatrix();
	glMultMatrixf((GLfloat*)transform.Transposed().ptr());

	glColor3f(color.r, color.g, color.b);
	glLineWidth(1.0f);

	glBegin(GL_LINES);
	
	glVertex3f(sphere.r, 0.0f, 0.0f);
	for (int i = 0; i < 4 * num_arc_points; i++)
	{
		float angle = angle_interval * i;
		float cosangle = cos(angle);
		float sinangle = sin(angle);
		glVertex3f(sphere.r * cosangle, 0.0f, sphere.r * sinangle);
		glVertex3f(sphere.r * cosangle, 0.0f, sphere.r * sinangle);
	}
	glVertex3f(sphere.r, 0.0f, 0.0f);

	glVertex3f(sphere.r, 0.0f, 0.0f);
	for (int i = 0; i < 4 * num_arc_points; i++)
	{
		float angle = angle_interval * i;
		float cosangle = cos(angle);
		float sinangle = sin(angle);
		glVertex3f(sphere.r * cosangle, sphere.r * sinangle, 0.0f);
		glVertex3f(sphere.r * cosangle, sphere.r * sinangle, 0.0f);
	}
	glVertex3f(sphere.r, 0.0f, 0.0f);

	glVertex3f(0.0f, sphere.r, 0.0f);
	for (int i = 0; i < 4 * num_arc_points; i++)
	{
		float angle = angle_interval * i;
		float cosangle = cos(angle);
		float sinangle = sin(angle);
		glVertex3f(0.0f, sphere.r * cosangle, sphere.r * sinangle);
		glVertex3f(0.0f, sphere.r * cosangle, sphere.r * sinangle);
	}
	glVertex3f(0.0f, sphere.r, 0.0f);

	glEnd();

	glPopMatrix();
}

void RenderDebugDrawer::DrawCapsule(const Capsule& capsule, const Color& color, const float4x4& transform)
{
	float half_segment = 0.5f * capsule.LineLength();

	glPushMatrix();
	glMultMatrixf((GLfloat*)transform.Transposed().ptr());

	glColor3f(color.r, color.g, color.b);
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	glVertex3f(capsule.r, half_segment, 0.0f);
	glVertex3f(capsule.r, -half_segment, 0.0f);

	glVertex3f(-capsule.r, half_segment, 0.0f);
	glVertex3f(-capsule.r, -half_segment, 0.0f);

	glVertex3f(0.0f, half_segment, capsule.r);
	glVertex3f(0.0f, -half_segment, capsule.r);

	glVertex3f(0.0f, half_segment, -capsule.r);
	glVertex3f(0.0f, -half_segment, -capsule.r);

	glEnd();

	glPopMatrix();

	DrawHalfSphere(capsule.SphereA(), color, true, transform.Translate(capsule.l.a));
	DrawHalfSphere(capsule.SphereB(), color, false, transform.Translate(capsule.l.b));
}

void RenderDebugDrawer::DrawHalfSphere(const Sphere& sphere, const Color& color, bool north_hemisfere, const float4x4& transform)
{
	int num_arc_points = 4;
	float angle_interval = (90 / num_arc_points) * DEG_TO_RAD;

	glPushMatrix();
	glMultMatrixf((GLfloat*)transform.Transposed().ptr());

	glColor3f(color.r, color.g, color.b);
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	glVertex3f(sphere.r, 0.0f, 0.0f);
	for (int i = 0; i < 4 * num_arc_points; i++)
	{
		float angle = angle_interval * i;
		float cosangle = cos(angle);
		float sinangle = sin(angle);
		glVertex3f(sphere.r * cosangle, 0.0f, sphere.r * sinangle);
		glVertex3f(sphere.r * cosangle, 0.0f, sphere.r * sinangle);
	}
	glVertex3f(sphere.r, 0.0f, 0.0f);

	if (north_hemisfere)
	{
		glVertex3f(sphere.r, 0.0f, 0.0f);
		for (int i = 0; i < 2 * num_arc_points; i++)
		{
			float angle = angle_interval * i;
			float cosangle = cos(angle);
			float sinangle = sin(angle);
			glVertex3f(sphere.r * cosangle, sphere.r * sinangle, 0.0f);
			glVertex3f(sphere.r * cosangle, sphere.r * sinangle, 0.0f);
		}
		glVertex3f(-sphere.r, 0.0f, 0.0f);

		glVertex3f(0.0f, 0.0f, sphere.r);
		for (int i = 0; i < 2 * num_arc_points; i++)
		{
			float angle = angle_interval * i;
			float cosangle = cos(angle);
			float sinangle = sin(angle);
			glVertex3f(0.0f, sphere.r * sinangle, sphere.r * cosangle);
			glVertex3f(0.0f, sphere.r * sinangle, sphere.r * cosangle);
		}
		glVertex3f(0.0f, 0.0f, -sphere.r);
	}
	else
	{
		glVertex3f(sphere.r, 0.0f, 0.0f);
		for (int i = 0; i < 2 * num_arc_points; i++)
		{
			float angle = angle_interval * i;
			float cosangle = cos(angle);
			float sinangle = sin(angle);
			glVertex3f(sphere.r * cosangle, -sphere.r * sinangle, 0.0f);
			glVertex3f(sphere.r * cosangle, -sphere.r * sinangle, 0.0f);
		}
		glVertex3f(-sphere.r, 0.0f, 0.0f);

		glVertex3f(0.0f, 0.0f, sphere.r);
		for (int i = 0; i < 2 * num_arc_points; i++)
		{
			float angle = angle_interval * i;
			float cosangle = cos(angle);
			float sinangle = sin(angle);
			glVertex3f(0.0f, -sphere.r * sinangle, sphere.r * cosangle);
			glVertex3f(0.0f, -sphere.r * sinangle, sphere.r * cosangle);
		}
		glVertex3f(0.0f, 0.0f, -sphere.r);
	}

	glEnd();

	glPopMatrix();
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