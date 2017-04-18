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
class Primitive;

class ModuleLevel : public Module
{
public:
	ModuleLevel();
	~ModuleLevel();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	void Draw() const;

	GameObject* CreateGameObject(const std::string& name = "GameObject", GameObject* parent = nullptr, GameObject* root_object = nullptr);
	GameObject* CreateGameObject(Primitive* primitive, const std::string& name = "GameObject", GameObject* parent = nullptr, GameObject* root_object = nullptr);
	GameObject* CreateGameObject(const char* texture, Primitive* primitive, const std::string& name = "GameObject", GameObject* parent = nullptr, GameObject* root_object = nullptr);

	GameObject* ImportScene(const char* folder, const char* file, bool is_dynamic = false);

	GameObject* GetRoot() { return root; }
	const GameObject* GetRoot() const { return root; }

	void SaveGameObjectsTransforms();
	void RestoreGameObjectsTransforms();

private:
	GameObject* RecursiveLoadSceneNode(aiNode* scene_node, const aiScene* scene, GameObject* parent, const aiString& folder_path, GameObject* root_scene_object, bool is_dynamic = false);

	void GetGLError(const char* string) const;

private:
	GameObject* root = nullptr;
	Quadtree* quadtree = nullptr;
};

#endif // !MODULELEVEL_H
