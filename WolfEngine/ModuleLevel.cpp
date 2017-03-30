#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleLevel.h"
#include "GameObject.h"
#include "OpenGL.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Math.h"
#include "Quadtree.h"

#pragma comment(lib, "assimp/libx86/assimp-vc140-mt.lib")

ModuleLevel::ModuleLevel() : Module(MODULE_LEVEL)
{
}

ModuleLevel::~ModuleLevel()
{
}

bool ModuleLevel::Init()
{
	APPLOG("Init level.");

	root = CreateGameObject("Root");

	quadtree = new Quadtree(AABB(float3(-100, -20, -100), float3(100, 20, 100)));

	return true;
}

update_status ModuleLevel::Update(float dt)
{
	root->Update();

	return UPDATE_CONTINUE;
}

bool ModuleLevel::CleanUp()
{
	APPLOG("Destroying GameObjects and clearing level.")

	RELEASE(root);

	RELEASE(quadtree);

	return true;
}

void ModuleLevel::Draw() const
{
	//quadtree->Draw();

	for (std::vector<GameObject*>::const_iterator it = root->childs.begin(); it != root->childs.end(); ++it)
	{
		if ((*it)->IsActive())
		{
			(*it)->Draw();
			(*it)->DrawHierarchy();
		}
	}
}

GameObject* ModuleLevel::CreateGameObject(const std::string& name, GameObject* parent, GameObject* root_object)
{
	if (parent == nullptr)
		parent = root;

	GameObject* ret = new GameObject(parent, root_object, name);

	return ret;
}

GameObject* ModuleLevel::ImportScene(const char * folder, const char * file)
{
	GameObject* res = nullptr;
	aiString folder_path = aiString();
	folder_path.Append(folder);

	aiString file_path = aiString(folder_path);
	file_path.Append(file);

	const aiScene* scene = aiImportFile(file_path.data, aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr)
	{
		res = RecursiveLoadSceneNode(scene->mRootNode, scene, root, folder_path, nullptr);
	}
	res->LoadBones();

	aiReleaseImport(scene);
	return res;
}

GameObject* ModuleLevel::RecursiveLoadSceneNode(aiNode* scene_node, const aiScene* scene, GameObject* parent, const aiString& folder_path, GameObject* root_scene_object)
{
	GameObject* new_object = CreateGameObject(scene_node->mName.data, parent, root_scene_object);
	if (root_scene_object == nullptr)
		root_scene_object = new_object;

	//Create transformation component
	aiVector3D ai_position;
	aiVector3D ai_scaling;
	aiQuaternion ai_rotation;
	scene_node->mTransformation.Decompose(ai_scaling, ai_rotation, ai_position);
	float3 position = float3(ai_position.x, ai_position.y, ai_position.z);
	float3 scaling = float3(ai_scaling.x, ai_scaling.y, ai_scaling.z);
	Quat rotation = Quat(ai_rotation.x, ai_rotation.y, ai_rotation.z, ai_rotation.w);
	new_object->SetLocalTransform(position, scaling, rotation);

	//Create mesh and materials
	if (scene_node->mNumMeshes > 1)
	{
		GameObject* mesh_object = nullptr;
		//Create one new game object for each mesh
		for (int i = 0; i < scene_node->mNumMeshes; i++)
		{
			mesh_object = CreateGameObject(scene_node->mName.data, new_object, root_scene_object);
			mesh_object->LoadMeshFromScene(scene->mMeshes[scene_node->mMeshes[i]], scene, folder_path);
		}
	}
	else if (scene_node->mNumMeshes == 1)
	{
		//Create mesh component on this object
		new_object->LoadMeshFromScene(scene->mMeshes[scene_node->mMeshes[0]], scene, folder_path);
	}

	for (int i = 0; i < scene_node->mNumChildren; i++)
		RecursiveLoadSceneNode(scene_node->mChildren[i], scene, new_object, folder_path, root_scene_object);
	return new_object;
}

void ModuleLevel::GetGLError(const char* string) const
{
	bool ret = true;

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		APPLOG("OpenGL error during %s: %s", string, gluErrorString(err));
		ret = false;
	}

}