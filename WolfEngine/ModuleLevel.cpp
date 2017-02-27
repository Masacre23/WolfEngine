#include "Application.h"
#include "ModuleLevel.h"
#include "ModuleTextures.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

ModuleLevel::ModuleLevel() : Module(MODULE_LEVEL)
{
	root = new Node();
	root->name = "Root";
}

ModuleLevel::~ModuleLevel()
{
	CleanChildren(root);
}

void ModuleLevel::CleanChildren(Node* parent)
{
	for (std::vector<Node*>::iterator it = parent->childs.begin(); it != parent->childs.end(); ++it)
		CleanChildren(*it);

	parent->childs.clear();
	RELEASE(parent);
}

void ModuleLevel::Load(const char * folder, const char * file)
{
	aiString folder_path = aiString();
	folder_path.Append(folder);

	aiString file_path = aiString(folder_path);
	file_path.Append(file);

	const aiScene* scene = aiImportFile(file_path.data, aiProcess_Triangulate);

	//Load materials
	size_t material_offset = materials.size();
	for (unsigned i = 0; i < scene->mNumMaterials; i++)
		LoadMaterial(scene->mMaterials[i], folder_path);

	//Load meshes
	size_t mesh_offset = meshes.size();
	for (unsigned i = 0; i < scene->mNumMeshes; i++)
		LoadMesh(scene->mMeshes[i], material_offset);

	//Load root module of aiScene
	Node* root_node = new Node();
	root_node->name = scene->mRootNode->mName.data;
	LinkNode(root_node, root);
	for (unsigned i = 0; i < scene->mRootNode->mNumMeshes; i++)
		root_node->meshes.push_back(scene->mRootNode->mMeshes[i] + mesh_offset);
	for (unsigned i = 0; i < scene->mRootNode->mNumChildren; i++)
		LoadChildren(scene->mRootNode->mChildren[i], root_node, mesh_offset);
}

void ModuleLevel::LoadMaterial(aiMaterial* scene_material, aiString folder_path)
{
	Material material;
	scene_material->Get(AI_MATKEY_COLOR_AMBIENT, material.ambient);
	scene_material->Get(AI_MATKEY_COLOR_DIFFUSE, material.diffuse);
	scene_material->Get(AI_MATKEY_COLOR_SPECULAR, material.specular);
	if (scene_material->Get(AI_MATKEY_SHININESS, material.shiness) == AI_SUCCESS)
		material.shiness *= 128.0f;
	float shine_strength = 1.0f;
	if (scene_material->Get(AI_MATKEY_SHININESS_STRENGTH, shine_strength) == AI_SUCCESS)
		material.specular *= shine_strength;

	aiString path;
	if (scene_material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
	{
		aiString full_path = aiString(folder_path);
		full_path.Append(path.data);

		material.texture = App->textures->LoadTexture(full_path);
	}

	materials.push_back(material);
}

void ModuleLevel::LoadMesh(aiMesh* scene_mesh, size_t material_offset)
{
	Mesh mesh;
	mesh.material = scene_mesh->mMaterialIndex + material_offset;
	mesh.num_vertices = scene_mesh->mNumVertices;
	mesh.vertices = new aiVector3D[mesh.num_vertices];
	for (int i = 0; i < mesh.num_vertices; i++)
		mesh.vertices[i] = scene_mesh->mVertices[i];
	mesh.normals = new aiVector3D[mesh.num_vertices];
	for (int i = 0; i < mesh.num_vertices; i++)
		mesh.normals[i] = scene_mesh->mNormals[i];
	mesh.tex_coords = new aiVector3D[mesh.num_vertices];
	for (int i = 0; i < mesh.num_vertices; i++)
		mesh.tex_coords[i] = scene_mesh->mTextureCoords[i][0];

	mesh.num_indices = scene_mesh->mNumFaces;
	mesh.indices = new unsigned int[3 * scene_mesh->mNumFaces];
	unsigned int c = 0;
	for (int j = 0; j < scene_mesh->mNumFaces; ++j)
	{
		for (int k = 0; k < 3; ++k)
		{
			mesh.indices[c++] = scene_mesh->mFaces[j].mIndices[k];
		}
	}
	if (c != 3 * scene_mesh->mNumFaces)
		LOG("Error loading meshes: Incorrect number of indices");

	meshes.push_back(mesh);
}

void ModuleLevel::LoadChildren(aiNode* scene_node, Node* parent, size_t mesh_offset)
{
	Node* node = new Node();
	node->name = scene_node->mName.data;
	LinkNode(node, parent);
	for (unsigned i = 0; i < scene_node->mNumMeshes; i++)
		node->meshes.push_back(scene_node->mMeshes[i] + mesh_offset);
	for (unsigned i = 0; i < scene_node->mNumChildren; i++)
		LoadChildren(scene_node->mChildren[i], node, mesh_offset);
}

void ModuleLevel::Clear()
{
}

void ModuleLevel::Draw()
{
}

Node* ModuleLevel::FindNode(const char * name)
{
	return nullptr;
}

void ModuleLevel::LinkNode(Node * node, Node * destination)
{
}

