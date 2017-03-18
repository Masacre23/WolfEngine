#ifndef MODULELEVEL_H
#define MODULELEVEL_H

#define MODULE_LEVEL "ModuleLevel"

#include "Module.h"
#include <vector>
#include <string>

struct aiScene;
struct aiString;
struct aiNode;

class GameObject;

class ModuleLevel : public Module
{
public:
	ModuleLevel();
	~ModuleLevel();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	void Draw() const;

	GameObject* CreateGameObject(GameObject* parent = nullptr, const std::string& name = "GameObject");

	GameObject* ImportScene(const char* folder, const char* file);

	GameObject* GetRoot() { return root; }
	const GameObject* GetRoot() const { return root; }

private:
	GameObject* RecursiveLoadSceneNode(aiNode* scene_node, const aiScene* scene, GameObject* parent, const aiString& folder_path);

	void GetGLError(const char* string) const;

private:
	GameObject* root = nullptr;
};

#endif // !MODULELEVEL_H
