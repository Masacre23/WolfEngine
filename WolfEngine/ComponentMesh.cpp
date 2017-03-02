#include "ComponentMesh.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "OpenGL.h"

ComponentMesh::ComponentMesh()
{
}

ComponentMesh::ComponentMesh(aiMesh * mesh)
{
	num_vertices = mesh->mNumVertices;
	vertices = new float3[num_vertices];
	for (size_t i = 0; i < num_vertices; ++i)
		vertices[i] = (float3) mesh->mVertices[i][0];

	has_normals = mesh->HasNormals();
	if (has_normals) {
		normals = new float3[num_vertices];
		for (size_t i = 0; i < num_vertices; ++i)
			normals[i] = (float3)mesh->mNormals[i][0];
	}
	
	has_tex_coords = mesh->HasTextureCoords(0);

	if (has_tex_coords) {
		tex_coords = new float2[num_vertices];
		for (size_t i = 0; i < num_vertices; ++i)
			tex_coords[i] = (float2)mesh->mTextureCoords[0][i][0];
	}

	num_indices = 3 * mesh->mNumFaces;
	indices = new unsigned int[num_indices];
	
	unsigned int c = 0;
	for (size_t j = 0; j < mesh->mNumFaces; ++j)
	{
		for (size_t k = 0; k < 3; ++k)
		{
			indices[c++] = mesh->mFaces[j].mIndices[k];
		}
	}
	if (c != 3 * mesh->mNumFaces)
		LOG("Error loading meshes: Incorrect number of indices");
}

ComponentMesh::~ComponentMesh()
{
}

bool ComponentMesh::OnUpdate()
{
	if (enable) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glNormalPointer(GL_FLOAT, 0, normals);
		glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);

		glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, indices);

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	return true;
}

bool ComponentMesh::OnEditor()
{
	return true;
}
