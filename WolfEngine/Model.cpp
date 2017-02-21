#include "Model.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "OpenGL.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "Material.h"
#include <string>

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

	// Load texture and materials.
	textures = new unsigned int *[scene->mNumMaterials];
	materials = new Material [scene->mNumMaterials];
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
				aiString full_path = aiString(folder_path);
				full_path.Append(path.data);

				textures[i][j] = App->textures->LoadTexture(full_path);
			}
		}

		float shine_strength = 1.0f;
		materials[i].has_ambient = material->Get(AI_MATKEY_COLOR_AMBIENT, materials[i].ambient);
		materials[i].has_diffuse = material->Get(AI_MATKEY_COLOR_DIFFUSE, materials[i].diffuse);
		materials[i].has_specular = material->Get(AI_MATKEY_COLOR_SPECULAR, materials[i].specular);
		materials[i].has_ambient = material->Get(AI_MATKEY_COLOR_AMBIENT, materials[i].ambient);
		materials[i].has_shiness = material->Get(AI_MATKEY_SHININESS, materials[i].shiness);
		bool has_strength = material->Get(AI_MATKEY_SHININESS_STRENGTH, shine_strength);

		for (int j = 0; j < 4; j++) {
			materials[i].specular[j] *= shine_strength;
		}
		materials[i].shiness *= 128.0f;

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
	RELEASE_ARRAY(materials);

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


		glMaterialfv(GL_FRONT, GL_AMBIENT, materials[mesh->mMaterialIndex].ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materials[mesh->mMaterialIndex].diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, materials[mesh->mMaterialIndex].specular);
		glMaterialf(GL_FRONT, GL_SHININESS, materials[mesh->mMaterialIndex].shiness);
		
		glDrawElements(GL_TRIANGLES, 3.0f * mesh->mNumFaces, GL_UNSIGNED_INT,  indices[i]);
		
		glBindTexture(GL_TEXTURE_2D, 0);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}


}
