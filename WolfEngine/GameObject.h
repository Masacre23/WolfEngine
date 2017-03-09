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
	void DrawHierarchy() const;

	void SetParent(GameObject* parent);
	Component* CreateComponent(Component::Type type);

	int GetNumComponents() const { return components.size(); }
	int GetNumChilds() const { return childs.size(); }
	const Component* GetComponent(Component::Type type) const;

	bool IsActive() const { return active; }

	void SetTransform(const float3& position, const float3& scaling, const Quat& rotation);
	void LoadMeshFromScene(aiMesh* scene_mesh, const aiScene* scene, const aiString& folder_path);

private:
	void RecursiveDrawHierarchy(const float4x4& parent_transform) const;
	void DrrawAABBBox();

public:
	std::string name = "GameObject";
	std::vector<Component*> components;
	std::vector<GameObject*> childs;

	Component* transform = nullptr;
	Component* mesh = nullptr;
	Component* material = nullptr;
	AABB bbox;

private:
	GameObject* parent = nullptr;
	bool active;
};



#endif // !GAMEOBJECT_H
