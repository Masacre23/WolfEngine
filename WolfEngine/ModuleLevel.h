#ifndef MODULELEVEL_H
#define MODULELEVEL_H

#define MODULE_LEVEL "ModuleLevel"

#include "Module.h"
#include <vector>
#include <string>
#include <assimp/types.h>

struct Node
{
	std::string name;
	aiVector3D position = aiVector3D(0, 0, 0);
	aiQuaternion rotation = aiQuaternion(1, 0, 0, 0);
	std::vector<unsigned> meshes;
	Node* parent = nullptr;
	std::vector<Node*> childs;
};

struct Mesh
{
	unsigned material = 0;
	aiVector3D* vertices = nullptr;
	aiVector3D* tex_coords = nullptr;
	aiVector3D* normals = nullptr;
	unsigned num_vertices = 0;
	unsigned* indices = 0;
	unsigned num_indices = 0;
};

struct Material
{
	aiColor4D ambient = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
	aiColor4D diffuse = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
	aiColor4D specular = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
	float shiness = 0.0f;
	unsigned texture = 0;
};

class ModuleLevel : public Module
{
public:
	ModuleLevel();
	~ModuleLevel();

	void Load(const char* folder, const char* file);
	void Clear();
	void Draw();

	unsigned GetNumMeshes() const { return meshes.size(); }
	unsigned GetNumMaterials() const { return materials.size(); }

	Mesh& GetMesh(unsigned index) { return meshes[index]; }
	const Mesh& GetMesh(unsigned index) const { return meshes[index]; }

	Material& GetMaterial(unsigned index) { return materials[index]; }
	const Material& GetMaterial(unsigned index) const { return materials[index]; }

	Node* GetRootNode() { return root; }
	const Node* GetRootNode() const { return root; }

	Node* FindNode(const char* name);
	void LinkNode(Node* node, Node* destination);

private:
	Node* root = nullptr;
	std::vector<Mesh> meshes;
	std::vector<Material> materials;
};

#endif // !MODULELEVEL_H
