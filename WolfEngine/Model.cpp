#include "Model.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "OpenGL.h"

#include "Application.h"
#include "ModuleTextures.h"
#include <vector>

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
	indices_size = new  int [scene->mNumMeshes];
	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];
		int cant_indicies = 0;
		for (int j = 0; j < mesh->mNumFaces; ++j)
		{
			cant_indicies += mesh->mFaces[j].mNumIndices;
		}
		indices[i] = new unsigned int[cant_indicies];
		indices_size[i] = cant_indicies;
		int c = 0;
		for (int j = 0; j < mesh->mNumFaces; ++j) {
			for (int k = 0; k < mesh->mFaces[j].mNumIndices; ++k)
			{
				indices[i][c++] = mesh->mFaces[j].mIndices[k];
			}
		}
	}
}

void Model::Clear()
{
	aiReleaseImport(scene);
}

void Model::Draw()
{
	////CVertexBufferObject vboModelData;
	//glBegin(GL_TRIANGLES);
	//for (int i = 0; i < scene->mNumMeshes; ++i)
	//{
	//	aiMesh* mesh = scene->mMeshes[i];
	//	for (int j = 0; j < mesh->mNumFaces; ++j) 
	//	{
	//		aiFace face = mesh->mFaces[j];
	//		for (int k = 0; k < face.mNumIndices; ++k) 
	//		{
	//			aiVector3D pos = mesh->mVertices[face.mIndices[k]];
	//			//aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
	//			aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);
	//			glNormal3f(normal.x, normal.y, normal.z);
	//			glVertex3f(pos.x, pos.y, pos.z);
	//			const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	//			const aiVector3D pTexCoord = mesh->mTextureCoords[0][k];

	//			//glTexCoord2f(uv.x, uv.y);
	//		}
	//	}
	//}

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
	/*glEnd();*/

	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, mesh->mVertices);
		
		/*glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, mesh->mNormals);*/
		
		/*glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(mesh->mNumUVComponents[0], GL_FLOAT, 0, mesh->mTextureCoords[0]);*/
		
		glDrawElements(GL_TRIANGLES, indices_size[i], GL_UNSIGNED_INT,  indices[i]);
		
	/*	glDisableClientState(GL_TEXTURE_COORD_ARRAY);*/
		/*glDisableClientState(GL_NORMAL_ARRAY);*/
		glDisableClientState(GL_VERTEX_ARRAY);
	}


}
