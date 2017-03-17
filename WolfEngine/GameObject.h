#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <string>
#include "Component.h"
#include "Math.h"

class Component;
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;

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

	float4x4 GetGlobalTransformMatrix();

private:
	void RecursiveDrawHierarchy() const;
	void DrawAABBBox() const;

public:
	AABB bbox;
	std::string name = "GameObject";
	std::vector<Component*> components;
	std::vector<GameObject*> childs;

	ComponentTransform * transform = nullptr;
	ComponentMesh* mesh = nullptr;
	ComponentMaterial* material = nullptr;

	bool selected = false;

private:
	GameObject* parent = nullptr;
	bool active;
};

#endif // !GAMEOBJECT_H