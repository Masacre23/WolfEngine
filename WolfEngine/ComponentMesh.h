#ifndef COMPONENTMESH_H
#define COMPONENTMESH_H

#include "Component.h"
#include "Math.h"
#include <vector>
#include <assimp/types.h>

struct aiMesh;

struct Weight
{
	unsigned vertex = 0;
	float weight = 0.0f;
};

struct Bone
{
	aiString name;
	Weight* weights = nullptr;
	unsigned num_weights = 0;
	float4x4 bind;
};

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* parent);
	~ComponentMesh();

	void Load(aiMesh* mesh);

	bool OnUpdate();
	bool OnDraw() const;
	bool OnEditor(int selection_mask, int id);

private:
	float* vertices_bind = nullptr;
	float* vertices = nullptr;
	bool has_tex_coords = false;
	float* tex_coords = nullptr;
	bool has_normals = false;
	float* normals = nullptr;
	unsigned num_vertices = 0;
	unsigned* indices = nullptr;
	unsigned num_indices = 0;

	bool has_bones = false;
	std::vector<Bone*> bones;
};

#endif

