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
struct aiNode;
struct aiScene;
struct aiString;

class GameObject
{
public:
	GameObject(GameObject* parent, GameObject* root_object, const std::string& name = "GameObject");
	~GameObject();

	bool Update();
	
	void Draw() const;
	void DrawHierarchy() const;

	void SetParent(GameObject* parent);
	Component* CreateComponent(Component::Type type);

	int GetNumComponents() const { return components.size(); }
	int GetNumChilds() const { return childs.size(); }
	const Component* GetComponent(Component::Type type) const;
	void GetComponents(Component::Type type, std::vector<Component*>& components) const;
	const GameObject* FindByName(const std::string& name) const;

	bool IsActive() const { return active; }

	void SetLocalTransform(const float3& position, const float3& scaling, const Quat& rotation);
	void SetLocalTransformNoScale(const float3& position, const Quat& rotation);
	void LoadMeshFromScene(aiMesh* scene_mesh, const aiScene* scene, const aiString& folder_path);
	void LoadAnim(const char * name, const char * file);
	void LoadBones();

	void ChangeAnim(const char* name, unsigned int duration);

	void RecursiveGetGlobalTransformMatrix(float4x4& global_transform) const;
	void RecursiveGetBoneGlobalTransformMatrix(float4x4& global_transform) const;
	const float4x4& GetLocalTransformMatrix() const;

private:
	void RecursiveDrawHierarchy() const;

public:
	AABB bbox;
	std::string name = "GameObject";
	std::vector<Component*> components;
	std::vector<GameObject*> childs;

	ComponentTransform * transform = nullptr;
	ComponentMesh* mesh = nullptr;
	ComponentMaterial* material = nullptr;

	bool selected = false;
	bool active = true;

	GameObject* root = nullptr;

private:
	GameObject* parent = nullptr;
	
};

#endif // !GAMEOBJECT_H