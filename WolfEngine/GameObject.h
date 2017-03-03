#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <string>
#include "Component.h"
#include "Math.h"

class Component;

struct aiMesh;
struct aiScene;
struct aiString;

class GameObject
{
public:
	GameObject(GameObject* parent, const std::string& name = "GameObject");
	~GameObject();

	bool Update();
	
	void Draw() const;

	void SetParent(GameObject* parent);
	Component* CreateComponent(Component::Type type);

	int GetNumComponents() const { return components.size(); }
	int GetNumChilds() const { return childs.size(); }
	const Component* GetComponent(Component::Type type) const;
	const GameObject* GetChild(size_t index) const { return childs[index]; }
	bool IsActive() const { return active; }
	const std::string& GetName() const { return name; }

	void SetTransform(const float3& position, const float3& scaling, const Quat& rotation);
	void LoadMeshFromScene(aiMesh* scene_mesh, const aiScene* scene, const aiString& folder_path);

private:
	std::string name = "GameObject";
	GameObject* parent = nullptr;
	bool active;

	std::vector<Component*> components;
	std::vector<GameObject*> childs;
};



#endif // !GAMEOBJECT_H
