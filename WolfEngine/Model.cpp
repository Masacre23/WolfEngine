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

void Model::Load(const char * file)
{
	scene = aiImportFile(file, aiProcess_PreTransformVertices | aiProcess_FlipUVs);
	 
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
}

void Model::Clear()
{
	for (int i = 0;  i < scene->mNumMeshes;  i++)
	{
		RELEASE_ARRAY(indices[i]);
	}
	RELEASE_ARRAY(indices);

	aiReleaseImport(scene);
}

void Model::Draw()
{
	//for (int i = 0; i < scene->mNumMaterials; ++i)
	//{
	//	const aiMaterial* material = scene->mMaterials[i];
	//	//int a = 5;
	//	int texIndex = 0;
	//	aiString path;

	//	if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS)
	//	{
	//		//aiString sDir = aiString("Resources/Models/Batman/Batman_Cape_D");
	//		//aiString sTextureName = aiString(path.data);
	//		aiString sFullPath = aiString("Resources/Models/Batman/");
	//		sFullPath.Append(path.data);
	//		int iTexFound = -1;

	//		unsigned int texture = App->textures->LoadTexture(sFullPath.data);

	//		glBindTexture(GL_TEXTURE_2D, texture);
	//		//m_Textures[i] = NULL;
	//		
	//		//glTexCoord2f();

	//		GLuint id_texture = 0;
	//		/*if (texture == App->textures->texture_checkers)
	//			id_texture = id_texture_check;
	//		else
	//			id_texture = this->id_texture;*/

	//		glBindBuffer(GL_ARRAY_BUFFER, id_texture);
	//		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	//	}
	//}

	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, mesh->mVertices);
		glNormalPointer(GL_FLOAT, 0, mesh->mNormals);
		glTexCoordPointer(mesh->mNumUVComponents[0], GL_FLOAT, 0, mesh->mTextureCoords[0]);
		
		glDrawElements(GL_TRIANGLES, 3.0f * mesh->mNumFaces, GL_UNSIGNED_INT,  indices[i]);
		
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}


}
