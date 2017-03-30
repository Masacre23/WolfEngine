#ifndef COMPONENTMESH_H
#define COMPONENTMESH_H

#include "Component.h"
#include "Math.h"
#include <vector>
#include <assimp/types.h>

struct aiMesh;
//struct aiString;

struct Weight
{
	unsigned vertex = 0;
	float weight = 0.0f;
};

struct Bone
{
	aiString name;
	GameObject* bone_object = nullptr;
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
	void LoadBones();

	bool OnUpdate();
	bool OnDraw() const;
	bool OnEditor();

	void DrawNormals() const;

public:
	aiString folder_path;

private:
	unsigned vertices_id = 0;
	unsigned normals_id = 0;
	unsigned texture_id = 0;
	unsigned indices_id = 0;

	float3* vertices_bind = nullptr;
	float3* vertices = nullptr;
	bool has_tex_coords = false;
	float* tex_coords = nullptr;
	bool has_normals = false;
	float3* normals_bind = nullptr;
	float3* normals = nullptr;
	unsigned num_vertices = 0;
	unsigned* indices = nullptr;
	unsigned num_indices = 0;

	bool has_bones = false;
	int num_bones;
	Bone* bones;
};

#endif

