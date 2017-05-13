#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <list>
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
	void DebugDraw() const;
	void DrawHierarchy() const;

	void SetParent(GameObject* parent);
	Component* CreateComponent(Component::Type type);
	void DeleteComponent(Component* component);

	int GetNumComponents() const { return components.size(); }
	int GetNumChilds() const { return childs.size(); }

	const Component* GetComponent(Component::Type type, bool only_active = false) const;
	Component* GetComponent(Component::Type type, bool only_active = false);
	GameObject* FindByName(const std::string& name) const;

	bool IsActive() const { return active; }
	bool IsPlayingAnimation() const;

	void SetLocalTransform(const float3& position, const float3& scaling, const Quat& rotation);
	void SetLocalTransform(const float3& position, const Quat& rotation);
	void SetLocalTransform(const float3& position);
	void LoadMesh(aiMesh* scene_mesh, const aiScene* scene, const aiString& folder_path, bool is_dynamic = false);
	void LoadMesh(Primitive* primitive);
	void LoadMaterial(aiMesh* scene_mesh, const aiScene* scene, const aiString& folder_path);
	void LoadMaterial(const aiString& path);
	void LoadAnimation(const char * name);
	void LoadAnimation(const std::list<std::string>& animations);
	void LoadBones();
	void SetAABB(AABB box);

	void ChangeAnim(const char* name, unsigned int duration);

	void RecursiveUpdateTransforms(const float4x4& parent = float4x4::identity);
	void RecursiveUpdateBoundingBox(bool force_recalculation = false);
	void RecursiveSaveComponents();
	void RecursiveRestoreComponents();

	const float4x4& GetLocalTransformMatrix() const;
	const float4x4& GetGlobalTransformMatrix() const;

private:
	void RecursiveDrawHierarchy() const;

public:
	AABB initial_bbox;
	OBB transform_bbox;
	AABB bbox;
	std::string name = "GameObject";
	std::vector<Component*> components;
	std::vector<GameObject*> childs;

	ComponentTransform* transform = nullptr;
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