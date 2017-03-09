#ifndef COMPONENTMESH_H
#define COMPONENTMESH_H

#include "Component.h"
#include "Math.h"

struct aiMesh;

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
	float* vertices = nullptr;
	bool has_tex_coords = false;
	float* tex_coords = nullptr;
	bool has_normals = false;
	float* normals = nullptr;
	unsigned num_vertices = 0;
	unsigned* indices = nullptr;
	unsigned num_indices = 0;
};

#endif

