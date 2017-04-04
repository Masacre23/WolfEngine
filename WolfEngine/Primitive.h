#ifndef PRIMITIVE_H
#define PRIMITIVE_H

class Primitive
{
public:
	Primitive();
	~Primitive();

	void Init(unsigned num_vertices, unsigned num_indices, float* vertices, float* text_coord, float* normals, unsigned* indices);
	void Clear();

public:
	unsigned num_vertices;
	float* vertices;
	float* text_coord;
	float* normals;
	unsigned num_indices;
	unsigned* indices;
};

namespace Primitives
{
	extern Primitive Cube;
}
#endif // !PRIMITIVE_H
