#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleLevel.h"
#include "GameObject.h"
#include "OpenGL.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Math.h"

ModuleLevel::ModuleLevel() : Module(MODULE_LEVEL)
{
}

ModuleLevel::~ModuleLevel()
{
}

bool ModuleLevel::Init()
{
	LOG("Init level.");

	root = new GameObject(nullptr, "Root");

	return true;
}

bool ModuleLevel::CleanUp()
{
	LOG("Destroying GameObjects and clearing level.")

	RELEASE(root);

	return true;
}

void ModuleLevel::Draw() const
{
	//DrawNode(root);

	root->Draw();
}

GameObject* ModuleLevel::CreateGameObject(GameObject* parent, const std::string& name)
{
	if (parent == nullptr)
		parent = root;

	GameObject* ret = new GameObject(parent, name);

	return ret;
}

void ModuleLevel::ImportScene(const char * folder, const char * file)
{
	aiString folder_path = aiString();
	folder_path.Append(folder);

	aiString file_path = aiString(folder_path);
	file_path.Append(file);

	const aiScene* scene = aiImportFile(file_path.data, aiProcess_Triangulate);

	if (scene != nullptr)
	{
		RecursiveLoadSceneNode(scene->mRootNode, scene, root, folder_path);
	}

	aiReleaseImport(scene);
}

void ModuleLevel::RecursiveLoadSceneNode(aiNode* scene_node, const aiScene* scene, GameObject* parent, const aiString& folder_path)
{
	GameObject* new_object = new GameObject(parent, scene_node->mName.data);

	//Create transformation component
	aiVector3D ai_position;
	aiVector3D ai_scaling;
	aiQuaternion ai_rotation;
	scene_node->mTransformation.Decompose(ai_scaling, ai_rotation, ai_position);
	float3 position = float3(ai_position.x, ai_position.y, ai_position.z);
	float3 scaling = float3(ai_scaling.x, ai_scaling.y, ai_scaling.z);
	Quat rotation = Quat(ai_rotation.x, ai_rotation.y, ai_rotation.z, ai_rotation.w);
	new_object->SetTransform(position, scaling, rotation);

	//Create mesh and materials
	if (scene_node->mNumMeshes > 1)
	{
		GameObject* mesh_object = nullptr;
		//Create one new game object for each mesh
		for (int i = 0; i < scene_node->mNumMeshes; i++)
		{
			mesh_object = new GameObject(new_object, scene_node->mName.data);
			mesh_object->LoadMeshFromScene(scene->mMeshes[scene_node->mMeshes[i]], scene, folder_path);
		}
	}
	else if (scene_node->mNumMeshes == 1)
	{
		//Create mesh component on this object
		new_object->LoadMeshFromScene(scene->mMeshes[scene_node->mMeshes[0]], scene, folder_path);
	}

	for (int i = 0; i < scene_node->mNumChildren; i++)
		RecursiveLoadSceneNode(scene_node->mChildren[i], scene, new_object, folder_path);
}

void ModuleLevel::GetGLError(const char* string) const
{
	bool ret = true;

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		LOG("OpenGL error during %s: %s", string, gluErrorString(err));
		ret = false;
	}

}