#ifndef MODULELEVEL_H
#define MODULELEVEL_H

#define MODULE_LEVEL "ModuleLevel"

#include "Module.h"
#include <vector>
#include <string>
#include <assimp/types.h>
#include "MathGeoLib/src/Math/float3.h"
#include "MathGeoLib/src/Math/Quat.h"

struct aiNode;
struct aiMesh;
struct aiMaterial;
struct aiScene;

class GameObject;

class ModuleLevel : public Module
{
public:
	ModuleLevel();
	~ModuleLevel();

	bool Init();
	bool CleanUp();

	void Draw() const;

	GameObject* CreateGameObject(GameObject* parent = nullptr, const std::string& name = "GameObject");

	void ImportScene(const char* folder, const char* file);

	GameObject* GetRoot() { return root; }
	const GameObject* GetRoot() const { return root; }

private:
	void RecursiveLoadSceneNode(aiNode* scene_node, const aiScene* scene, GameObject* parent, const aiString& folder_path);

	void GetGLError(const char* string) const;

private:
	GameObject* root = nullptr;
};

#endif // !MODULELEVEL_H
