#include "Model.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "OpenGL.h"

#include "Application.h"
#include "ModuleTextures.h"

#pragma comment(lib, "assimp/libx86/assimp-vc140-mt.lib")

Model::Model()
{
}


Model::~Model()
{
}

void Model::Load(const char* folder, const char* file)
{
	aiString folder_path = aiString();
	folder_path.Append(folder);

	aiString file_path = aiString(folder_path);
	file_path.Append(file);

	scene = aiImportFile(file_path.data, aiProcess_PreTransformVertices);
	 
	indices = new unsigned int *[scene->mNumMeshes];
	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];
		indices[i] = new unsigned int[3.0f * mesh->mNumFaces];
		int c = 0;
		for (int j = 0; j < mesh->mNumFaces; ++j) 
		{
			for (int k = 0; k < mesh->mFaces[j].mNumIndices; ++k)
			{
				indices[i][c++] = mesh->mFaces[j].mIndices[k];
			}
		}
	}

	textures = new unsigned int *[scene->mNumMaterials];
	for (int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		unsigned int texIndex = 0;
		aiString path;
		aiTextureType type_texture = aiTextureType_DIFFUSE;

		textures[i] = new unsigned int[material->GetTextureCount(type_texture)];

		for (int j = 0; j < material->GetTextureCount(type_texture); j++)
		{
			if (material->GetTexture(type_texture, j, &path) == AI_SUCCESS)
			{
				aiString sFullPath = aiString(folder_path);
				sFullPath.Append(path.data);

				textures[i][j] = App->textures->LoadTexture(sFullPath.data);
			}
		}
		
	}
}

void Model::Clear()
{
	for (int i = 0;  i < scene->mNumMeshes;  i++)
	{
		RELEASE_ARRAY(indices[i]);
	}
	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(textures);

	aiReleaseImport(scene);
}

void Model::Draw()
{

	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, mesh->mVertices);
		glNormalPointer(GL_FLOAT, 0, mesh->mNormals);

		glBindTexture(GL_TEXTURE_2D, textures[mesh->mMaterialIndex][0]);
		glTexCoordPointer(2, GL_FLOAT, sizeof(aiVector3D), mesh->mTextureCoords[0]);
		
		glDrawElements(GL_TRIANGLES, 3.0f * mesh->mNumFaces, GL_UNSIGNED_INT,  indices[i]);
		
		glBindTexture(GL_TEXTURE_2D, 0);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}


}
