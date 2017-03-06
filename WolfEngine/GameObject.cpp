#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "OpenGL.h"

GameObject::GameObject(GameObject* parent, const std::string& name) : name(name)
{
	SetParent(parent);
}

GameObject::~GameObject()
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
		RELEASE(*it);

	for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
		RELEASE(*it);

}

bool GameObject::Update()
{
	return false;
}

void GameObject::Draw() const
{
	glPushMatrix();

	if (transform != nullptr)
		transform->OnDraw();
	
	glBindTexture(GL_TEXTURE_2D, 0);

	if (material != nullptr)
		material->OnDraw();

	if (mesh != nullptr)
		mesh->OnDraw();

	glBindTexture(GL_TEXTURE_2D, 0);

	for (std::vector<GameObject*>::const_iterator it = childs.begin(); it != childs.end(); ++it)
		if ((*it)->IsActive())
			(*it)->Draw();

	glPopMatrix();
}

void GameObject::SetParent(GameObject * parent)
{
	if (this->parent == parent || parent == nullptr)
		return;

	if (this->parent != nullptr)
	{
		std::vector<GameObject*>::iterator it = std::find(childs.begin(), childs.end(), this);
		if (it != childs.end())
		{
			childs.erase(it);
			LOG("Error detected: GameObject has a parent but it isn't in parent's childs' vector.")
		}
	}

	this->parent = parent;
	parent->childs.push_back(this);
}

Component* GameObject::CreateComponent(Component::Type type)
{
	static_assert(Component::Type::UNKNOWN == 3, "Update factory code");

	Component* ret = nullptr;

	switch (type)
	{
	case Component::TRANSFORM:
		ret = new ComponentTransform(this);
		transform = ret;
		break;
	case Component::MESH:
		ret = new ComponentMesh(this);
		mesh = ret;
		break;
	case Component::MATERIAL:
		ret = new ComponentMaterial(this);
		material = ret;
		break;
	case Component::UNKNOWN:
		break;
	default:
		break;
	}

	if (ret != nullptr)
		components.push_back(ret);

	return ret;
}

const Component * GameObject::GetComponent(Component::Type type) const
{
	Component* ret = nullptr;

	for (std::vector<Component*>::const_iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->GetType() == type && (*it)->IsActive())
			ret = *it;
	}

	return ret;
}

void GameObject::SetTransform(const float3& position, const float3& scaling, const Quat& rotation)
{
	if (transform == nullptr)
		transform = (ComponentTransform*)CreateComponent(Component::Type::TRANSFORM);

	((ComponentTransform*)transform)->Load(position, scaling, rotation);
}

void GameObject::LoadMeshFromScene(aiMesh* scene_mesh, const aiScene* scene, const aiString& folder_path)
{
	ComponentMesh* mesh = (ComponentMesh*)CreateComponent(Component::Type::MESH);
	mesh->Load(scene_mesh);

	ComponentMaterial* material = (ComponentMaterial*)CreateComponent(Component::Type::MATERIAL);
	aiMaterial* scene_material = scene->mMaterials[scene_mesh->mMaterialIndex];
	material->Load(scene_material, folder_path);
}
