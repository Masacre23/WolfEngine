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
class Quadtree;

class ModuleLevel : public Module
{
public:
	ModuleLevel();
	~ModuleLevel();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	void Draw() const;

	GameObject* CreateGameObject(const std::string& name = "GameObject", GameObject* parent = nullptr, GameObject* root_object = nullptr);

	GameObject* ImportScene(const char* folder, const char* file);

	GameObject* GetRoot() { return root; }
	const GameObject* GetRoot() const { return root; }

private:
	GameObject* RecursiveLoadSceneNode(aiNode* scene_node, const aiScene* scene, GameObject* parent, const aiString& folder_path, GameObject* root_scene_object);

	void GetGLError(const char* string) const;

private:
	GameObject* root = nullptr;
	Quadtree* quadtree = nullptr;
};

#endif // !MODULELEVEL_H
