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
class MyQuadTree;
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
	void DrawDebug() const;

	GameObject* CreateGameObject(const std::string& name = "GameObject", GameObject* parent = nullptr, GameObject* root_object = nullptr);
	GameObject* CreateGameObject(const Primitive& primitive, const std::string& name = "GameObject", GameObject* parent = nullptr, GameObject* root_object = nullptr);
	GameObject* CreateGameObject(const char* texture, const Primitive& primitive, const std::string& name = "GameObject", GameObject* parent = nullptr, GameObject* root_object = nullptr);

	GameObject* ImportScene(const char* folder, const char* file, bool is_dynamic = false);
	GameObject* AddCamera();

	GameObject* GetRoot() { return root; }
	const GameObject* GetRoot() const { return root; }

	void InsertGameObjectQuadTree(GameObject * game_object);
	void OnPlay();
	void OnStop();

private:
	GameObject* RecursiveLoadSceneNode(aiNode* scene_node, const aiScene* scene, GameObject* parent, const aiString& folder_path, GameObject* root_scene_object, bool is_dynamic = false);

	void GetGLError(const char* string) const;

public:
	bool draw_quadtree_structure = false;

private:
	GameObject* root = nullptr;
	GameObject* camera = nullptr;
	MyQuadTree* quadtree = nullptr;
};

#endif // !MODULELEVEL_H
