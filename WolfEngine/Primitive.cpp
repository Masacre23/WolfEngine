#include "Primitive.h"
#include "Globals.h"
#include "OpenGL.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Math.h"

Primitive::Primitive()
{
}

Primitive::Primitive(const float3& position, const Color& color) : position(position), color(color)
{
}

Primitive::~Primitive()
{
}

PrimitivePlane::PrimitivePlane() : Primitive(), lines_distance(1.0f)
{
	num_lines = 2;
	edges = 0.5f;

	type = Primitive::Type::PLANE;
	num_vertices = 4;
	num_indices = 6;
}

PrimitivePlane::PrimitivePlane(float width, float distance, const float3& position, const Color& color) : Primitive(position,color), lines_distance(distance), width(width)
{
	if (lines_distance <= 0.0f)
		lines_distance = 1.0f;

	num_lines = ((int)((width) / lines_distance)) + 1;
	edges = width * 0.5f;

	type = Primitive::Type::PLANE;
	num_vertices = 4;
	num_indices = 6;
}

void PrimitivePlane::Draw() const
{
	glLineWidth(2.0f);
	App->renderer->debug_drawer->SetColor(color);

	glBegin(GL_LINES);
	for (int i = 0; i < num_lines; i++)
	{
		glVertex3f(-edges + i*lines_distance, 0.0, -edges);
		glVertex3f(-edges + i*lines_distance, 0.0, edges);
		glVertex3f(-edges, 0.0, -edges + i*lines_distance);
		glVertex3f(edges, 0.0, -edges + i*lines_distance);
	}
	glEnd();
}

void PrimitivePlane::LoadMesh(float3* vertices, float2* tex_coord, float3* normals, unsigned* indices) const
{
	float half = width * 0.5f;

	float3 A = { -half, 0.0f, -half };
	float3 B = { half, 0.0f, -half };
	float3 C = { -half, 0.0f, half };
	float3 D = { half, 0.0f, half };

	float vert[] = { A.x, A.y, A.z,  B.x, B.y, B.z,  C.x, C.y, C.z,  D.x, D.y, D.z};
	float texture_coord[] = { 0,0, 1,0, 0,1, 1,1 };
	float norm[] = { 0,1,0, 0,1,0, 0,1,0, 0,1,0 };
	unsigned plane_indices[] = { 0, 2, 1,  2, 3, 1 };

	memcpy(vertices, vert, 3 * num_vertices * sizeof(float));
	memcpy(tex_coord, texture_coord, 2 * num_vertices * sizeof(float));
	memcpy(normals, norm, 3 * num_vertices * sizeof(float));
	memcpy(indices, plane_indices, num_indices * sizeof(unsigned));
}

PrimitiveCube::PrimitiveCube() : Primitive()
{
	type = Primitive::Type::CUBE;
	num_vertices = 12;
	num_indices = 36;
}

PrimitiveCube::PrimitiveCube(const float3& size, const float3& position, const Color& color) : Primitive(position, color), size(size)
{
	type = Primitive::Type::CUBE;
	num_vertices = 12;
	num_indices = 36;
}

void PrimitiveCube::LoadMesh(float3* vertices, float2* tex_coord, float3* normals, unsigned* indices) const
{
	float3 half = size * 0.5f;

	float3 A = { -half.x, -half.y, half.z };
	float3 B = { half.x, -half.y, half.z };
	float3 C = { -half.x, half.y, half.z };
	float3 D = { half.x, half.y, half.z };
	float3 E = { -half.x, -half.y, -half.z };
	float3 F = { half.x, -half.y, -half.z };
	float3 G = { -half.x, half.y, -half.z };
	float3 H = { half.x, half.y, -half.z };

	float vert[] = { A.x, A.y, A.z,  B.x, B.y, B.z,  C.x, C.y, C.z,  D.x, D.y, D.z, E.x, E.y, E.z, F.x, F.y, F.z, G.x, G.y, G.z, H.x, H.y, H.z, A.x, A.y, A.z,  B.x, B.y, B.z, G.x, G.y, G.z, H.x, H.y, H.z };
	float texture_coord[] = { 0,0, 1,0, 0,1, 1,1, 1,0, 0,0, 1,1, 0,1, 1,1, 0,1, 0,0, 1,0 };
	float norm[] = { -1,-1,1, 1,-1,1, -1,1,1, 1,1,1, -1,-1,-1, 1,-1,-1, -1,1,-1, 1,1,-1, -1,-1,1, 1,-1,1, -1,1,-1, 1,1,-1 };
	unsigned triangles_indices[] = { 0, 1, 2,  2, 1, 3,  3, 9, 5,  3, 5, 11,  10, 4, 8,  10, 8, 2,  2, 3, 10,  10, 3, 11,  6, 7, 4,  4, 7, 5,  4, 5, 8,  8, 5, 9 };

	memcpy(vertices, vert, 3 * num_vertices * sizeof(float));
	memcpy(tex_coord, texture_coord, 2 * num_vertices * sizeof(float));
	memcpy(normals, norm, 3 * num_vertices * sizeof(float));
	memcpy(indices, triangles_indices, num_indices * sizeof(unsigned));
}

PrimitiveSphere::PrimitiveSphere(float radius, const float3& position, const Color& color, unsigned int rings, unsigned int sectors) : Primitive(position, color), radius(radius), rings(rings), sectors(sectors)
{	
	type = Primitive::Type::SPHERE;
	num_vertices = rings * sectors;
	num_indices = (rings-1) * (sectors-1) * 6;
}

void PrimitiveSphere::LoadMesh(float3* vertices, float2* tex_coord, float3* normals, unsigned* indices) const
{
	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	int v = 0;
	int n = 0;
	int t = 0;
	for (r = 0; r < rings; r++) 
		for (s = 0; s < sectors; s++) 
		{
			float const y = sin(-pi/2 + pi * r * R);
			float const x = cos(2 * pi * s * S) * sin(pi * r * R);
			float const z = sin(2 * pi * s * S) * sin(pi * r * R);

			tex_coord[t].x = s*S;
			tex_coord[t].y = r*R;

			vertices[v].x = x * radius;
			vertices[v].y = y * radius;
			vertices[v].z = z * radius;

			normals[n].x = x;
			normals[n].y = y;
			normals[n].z = z;

			++t;
			++v;
			++n;
		}

	int i = 0;
	for (r = 0; r < rings-1; r++) 
		for (s = 0; s < sectors-1; s++) 
		{
			indices[i] = r * sectors + s;
			indices[i + 1] = (r + 1) * sectors + (s + 1);
			indices[i + 2] = r * sectors + (s + 1);
			indices[i + 3] = r * sectors + s;
			indices[i + 4] = (r + 1) * sectors + s;
			indices[i + 5] = (r + 1) * sectors + (s + 1);
		
			i += 6;
		}
}
