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
class ComponentBillboard;
class ComponentParticleSystem;
class Primitive;

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
	GameObject* FindByName(const std::string& name) const;

	bool IsActive() const { return active; }

	void SetLocalTransform(const float3& position, const float3& scaling, const Quat& rotation);
	void SetLocalTransform(const float3& position, const Quat& rotation);
	void SetLocalTransform(const float3& position);
	void LoadMesh(aiMesh* scene_mesh, const aiScene* scene, const aiString& folder_path);
	void LoadMesh(Primitive* primitive);
	void LoadMaterial(aiMesh* scene_mesh, const aiScene* scene, const aiString& folder_path);
	void LoadMaterial(const aiString& path);
	void LoadMaterial();
	void LoadAnim(const char * name, const char * file);
	void LoadBones();
	void SetAABB(AABB box);

	void ChangeAnim(const char* name, unsigned int duration);

	void UpdateGlobalTransforms();

	const float4x4& GetLocalTransformMatrix() const;
	const float4x4& GetGlobalTransformMatrix() const;

private:
	void RecursiveDrawHierarchy() const;
	void RecursiveUpdateTransforms(const float4x4& parent);

public:
	AABB initial_bbox;
	OBB transform_bbox;
	AABB bbox;
	std::string name = "GameObject";
	std::vector<Component*> components;
	std::vector<GameObject*> childs;

	ComponentTransform * transform = nullptr;
	ComponentMesh* mesh = nullptr;
	ComponentMaterial* material = nullptr;
	ComponentBillboard* billboard = nullptr;
	ComponentParticleSystem* particle_system = nullptr;

	bool selected = false;
	bool active = true;
	bool is_bone = false;

	GameObject* root = nullptr;

private:
	GameObject* parent = nullptr;
	
};

#endif // !GAMEOBJECT_H