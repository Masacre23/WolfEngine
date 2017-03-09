#include "ComponentMesh.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "OpenGL.h"
#include "GameObject.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(Component::Type::MESH, parent)
{
}

ComponentMesh::~ComponentMesh()
{
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(tex_coords);
	RELEASE_ARRAY(indices);
}

void ComponentMesh::Load(aiMesh * mesh)
{
	num_vertices = mesh->mNumVertices;
	vertices = new float[3 * num_vertices];
	unsigned c = 0;
	for (size_t i = 0; i < num_vertices; ++i)
		for (size_t j = 0; j < 3; ++j)
			vertices[c++] = mesh->mVertices[i][j];
	if (c != 3 * mesh->mNumVertices)
		LOG("Error loading meshes: Incorrect number of vertices");

	//Creating BoundingBox from vertices points
	parent->bbox.SetNegativeInfinity();
	parent->bbox.Enclose((float3*)vertices, num_vertices);

	has_normals = mesh->HasNormals();
	if (has_normals) 
	{
		normals = new float[3 * num_vertices];
		c = 0;
		for (size_t i = 0; i < num_vertices; ++i)
			for (size_t j = 0; j < 3; ++j)
				normals[c++] = mesh->mNormals[i][j];
		if (c != 3 * mesh->mNumVertices)
			LOG("Error loading meshes: Incorrect number of normals");
	}

	has_tex_coords = mesh->HasTextureCoords(0);
	if (has_tex_coords) 
	{
		tex_coords = new float[2 * num_vertices];
		c = 0;
		for (size_t i = 0; i < num_vertices; ++i)
			for (size_t j = 0; j < 2; ++j)
				tex_coords[c++] = mesh->mTextureCoords[0][i][j];
		if (c != 2 * mesh->mNumVertices)
			LOG("Error loading meshes: Incorrect number of texture coordinates");
	}

	num_indices = 3 * mesh->mNumFaces;
	indices = new unsigned[num_indices];

	c = 0;
	for (size_t j = 0; j < mesh->mNumFaces; ++j)
		for (size_t k = 0; k < 3; ++k)
			indices[c++] = mesh->mFaces[j].mIndices[k];
	if (c != 3 * mesh->mNumFaces)
		LOG("Error loading meshes: Incorrect number of indices");
}

bool ComponentMesh::OnUpdate()
{
	return true;
}

bool ComponentMesh::OnDraw() const
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);

	if (has_normals)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, normals);
	}
		
	if (has_tex_coords)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);
	}
		
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, indices);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	return true;
}

bool ComponentMesh::OnEditor(int selection_mask, int id)
{
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << id)) ? ImGuiTreeNodeFlags_Selected : 0);
	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)id, node_flags, "Mesh");

	if (node_open)
	{
		ImGui::Checkbox("Active", &enable);

		ImGui::TreePop();
	}

	return ImGui::IsItemClicked();
}
