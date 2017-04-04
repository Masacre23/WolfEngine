#include "Primitive.h"
#include "Globals.h"
#include "Math.h"

Primitive Primitives::Cube = Primitive();

Primitive::Primitive()
{
}

Primitive::~Primitive()
{
}

void Primitive::Init(unsigned num_vertices, unsigned num_indices, float* vertices, float* text_coord, float* normals, unsigned* indices)
{
	this->num_vertices = num_vertices;
	this->num_indices = num_indices;

	this->vertices = new float[3 * num_vertices];
	memcpy(this->vertices, vertices, 3 * num_vertices * sizeof(float));

	this->text_coord = new float[2 * num_vertices];
	memcpy(this->text_coord, text_coord, 2 * num_vertices * sizeof(float));

	this->normals = new float[3 * num_vertices];
	memcpy(this->normals, normals, 3 * num_vertices * sizeof(float));

	this->indices = new unsigned[num_indices];
	memcpy(this->indices, indices, num_indices * sizeof(unsigned));
}

void Primitive::Clear()
{
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(text_coord);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(indices);
}

