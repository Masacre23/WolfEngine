#ifndef COMPONENTMESH_H
#define COMPONENTMESH_H

#include "Component.h"
#include "Math.h"
#include <assimp/types.h>

struct aiMesh;

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	void Load(aiMesh* mesh);

	bool OnUpdate();
	bool OnEditor();

private:
	float3* vertices = nullptr;
	bool has_tex_coords = false;
	float2* tex_coords = nullptr;
	bool has_normals = false;
	float3* normals = nullptr;
	unsigned num_vertices = 0;
	unsigned* indices = nullptr;
	unsigned num_indices = 0;
};

#endif

