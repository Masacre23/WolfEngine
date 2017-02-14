#include "Model.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
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
}

void Model::Clear()
{
	aiReleaseImport(scene);
}

void Model::Draw()
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		for (int j = 0; j < mesh->mNumFaces; j++) {
			aiFace face = mesh->mFaces[j];
			for (int k = 0; k < face.mNumIndices; k++) {
				aiVector3D pos = mesh->mVertices[face.mIndices[k]];
				glVertex3f(pos.x, pos.y, pos.z);
			}
		}
	}
	glEnd();
}
