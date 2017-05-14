#ifndef COMPONENTMESH_H
#define COMPONENTMESH_H

#include "Component.h"
#include "Math.h"
#include <vector>
#include <assimp/types.h>
#include "Glew/include/GL/glew.h"

class Primitive;

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

	void Load(aiMesh* mesh, bool is_dynamic = false);
	void Load(const Primitive& primitive);
	void LoadBones();

	bool OnUpdate();
	bool OnDraw() const;
	bool OnDebugDraw() const;
	bool OnEditor();

	void SaveComponent();
	void RestoreComponent();

	void DrawNormals() const;
	void DrawMesh() const;

	void SetUseNormals(bool material_on) { use_normals = has_normals && material_on; }

private:
	void SetAABB() const;

private:
	unsigned buffer_id = 0;

	unsigned vertices_id = 0;
	unsigned normals_id = 0;
	unsigned texture_id = 0;
	unsigned indices_id = 0;

	float* buffer = nullptr;

	float3* vertices = nullptr;
	bool has_tex_coords = false;
	float2* tex_coords = nullptr;
	bool has_normals = false;
	float3* normals = nullptr;
	unsigned num_vertices = 0;
	unsigned* indices = nullptr;
	unsigned num_indices = 0;

	bool has_bones = false;
	int num_bones;
	Bone* bones;

	bool use_normals = false;

	bool draw_normals = false;
	bool draw_mesh = false;

	GLenum draw_mode = GL_STATIC_DRAW;
};

#endif

