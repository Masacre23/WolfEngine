#include "Model.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "OpenGL.h"

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

	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];
		std::vector<unsigned int> mesh_indices;
		for (int j = 0; j < mesh->mNumFaces; ++j)
		{
			for (int k = 0; k < mesh->mFaces[j].mNumIndices; ++k)
			{
				mesh_indices.push_back(mesh->mFaces[j].mIndices[k]);
			}
		}
		indices.push_back(mesh_indices);
	}
}

void Model::Clear()
{
	aiReleaseImport(scene);
}

void Model::Draw()
{
	//glBegin(GL_TRIANGLES);

	
	/*for (int i = 0; i < scene->mNumMeshes; i++) 
	{
		aiMesh* mesh = scene->mMeshes[i];
		for (int j = 0; j < mesh->mNumFaces; j++) 
		{
			aiFace face = mesh->mFaces[j];
			for (int k = 0; k < face.mNumIndices; k++) 
			{
				aiVector3D pos = mesh->mVertices[face.mIndices[k]];
				aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);
				glNormal3f(normal.x, normal.y, normal.z);
				glVertex3f(pos.x, pos.y, pos.z);
			}
		}
	}*/

	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(mesh->mNumVertices, GL_FLOAT, 0, mesh->mVertices);
		
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, mesh->mNormals);
		
		/*glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(mesh->mNumUVComponents[0], GL_FLOAT, 0, mesh->mTextureCoords[0]);*/
		
		glDrawElements(GL_TRIANGLES, indices[i].size(), GL_UNSIGNED_INT, (void *) indices[i][0]);
		
	/*	glDisableClientState(GL_TEXTURE_COORD_ARRAY);*/
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	//glEnd();
}
