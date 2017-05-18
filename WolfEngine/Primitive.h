#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Math.h"
#include "Color.h"

class Primitive
{
public:
	enum Type
	{
		CUBE,
		PLANE,
		SPHERE
	};

	Primitive();
	Primitive(const float3& size, const Color& color = Colors::Black);
	Primitive(const float3& position, const float3& size, const Color& color = Colors::Black);
	~Primitive();

	const float3& GetPosition() const { return position; }
	const float3& GetSize() const { return size; }

	unsigned GetNumVertices() const { return num_vertices; }
	unsigned GetNumIndices() const { return num_indices; }

	virtual void Draw() const {}
	virtual void LoadMesh(float3* vertices, float2* tex_coord, float3* normals, unsigned* indices) const {}

protected:
	Type type;
	unsigned num_vertices;
	unsigned num_indices;

	float3 size = float3::one;
	float3 position = float3::zero;
	Color color = Colors::Black;
};

class PrimitivePlane : public Primitive
{
public:
	PrimitivePlane();
	PrimitivePlane(float width, float position_y, float distance, const Color& color);

	void Draw() const;

private:
	float lines_distance = 0.1f;
	float edges = 0.5f;
	int num_lines = 10;
};

class PrimitiveCube : public Primitive
{
public:
	PrimitiveCube();
	PrimitiveCube(const float3& size);
	PrimitiveCube(const float3& position, const float3& size);

	void LoadMesh(float3* vertices, float2* tex_coord, float3* normals, unsigned* indices) const;
};

class PrimitiveSphere : public Primitive
{
public:
	PrimitiveSphere(float radius, unsigned int rings = 20, unsigned int sectors = 20);

	void LoadMesh(float3* vertices, float2* tex_coord, float3* normals, unsigned* indices) const;

private:
	float radius;
	unsigned int rings;
	unsigned int sectors;
};


#endif // !PRIMITIVE_H
