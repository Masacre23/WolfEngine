#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ComponentAnim.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Application.h"
#include "ModuleAnimations.h"
#include "ModuleLevel.h"
#include "OpenGL.h"

GameObject::GameObject(GameObject* parent, GameObject* root_object, const std::string& name) : name(name), root(root_object)
{
	if (root_object == nullptr)
		root = this;

	SetParent(parent);
	components.push_back(transform = new ComponentTransform(this));

	//Init BoundingBox (in case some GameObjects don't have a MeshComponent)
	bbox.SetNegativeInfinity();
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
	//bbox.TransformAsAABB(GetGlobalTransformMatrix());
	for (std::vector<Component*>::const_iterator it = components.begin(); it != components.cend(); ++it)
		if ((*it)->IsActive())
			(*it)->OnUpdate();
	for (std::vector<GameObject*>::const_iterator it = childs.begin(); it != childs.end(); ++it)
		if ((*it)->IsActive())
			(*it)->Update();
	return true;
}

void GameObject::Draw() const
{
	glPushMatrix();

	if (selected)
		DrawAABBBox();

	if (transform != nullptr)
		if (transform->IsActive())
			transform->OnDraw();
	
	glBindTexture(GL_TEXTURE_2D, 0);

	if (material != nullptr)
		if (material->IsActive())
			material->OnDraw();

	if (mesh != nullptr)
		if (mesh->IsActive())
			mesh->OnDraw();

	glBindTexture(GL_TEXTURE_2D, 0);

	for (std::vector<GameObject*>::const_iterator it = childs.begin(); it != childs.end(); ++it)
		if ((*it)->IsActive())
			(*it)->Draw();

	glPopMatrix();

}

void GameObject::DrawHierarchy() const
{
	glLineWidth(2.0f);
	glDepthRange(0.0, 0.1);
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	
	glColor3f(0.0f, 0.0f, 1.0f);

	glPushMatrix();

	if (transform != nullptr)
		if (transform->IsActive())
			transform->OnDraw();

	for (std::vector<GameObject*>::const_iterator it = childs.cbegin(); it != childs.cend(); ++it)
	{
		if ((*it)->IsActive())
		{
			float3 child_transform = (*it)->transform->GetPosition();
			glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(child_transform.x, child_transform.y, child_transform.z);
			glEnd();
			(*it)->RecursiveDrawHierarchy();
		}	
	}

	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
}

void GameObject::RecursiveDrawHierarchy() const
{
	glPushMatrix();

	if (transform != nullptr)
		if (transform->IsActive())
			transform->OnDraw();

	for (std::vector<GameObject*>::const_iterator it = childs.cbegin(); it != childs.cend(); ++it)
	{
		float3 child_transform = (*it)->transform->GetPosition();
		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(child_transform.x, child_transform.y, child_transform.z);
		glEnd();
		(*it)->RecursiveDrawHierarchy();
	}

	glPopMatrix();	
}

void GameObject::DrawAABBBox() const 
{
	//Draw AABB box.
	glLineWidth(2.0f);
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_LINES);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(bbox.CornerPoint(0).x, bbox.CornerPoint(0).y, bbox.CornerPoint(0).z);
	glVertex3f(bbox.CornerPoint(1).x, bbox.CornerPoint(1).y, bbox.CornerPoint(1).z);

	glVertex3f(bbox.CornerPoint(0).x, bbox.CornerPoint(0).y, bbox.CornerPoint(0).z);
	glVertex3f(bbox.CornerPoint(2).x, bbox.CornerPoint(2).y, bbox.CornerPoint(2).z);

	glVertex3f(bbox.CornerPoint(0).x, bbox.CornerPoint(0).y, bbox.CornerPoint(0).z);
	glVertex3f(bbox.CornerPoint(4).x, bbox.CornerPoint(4).y, bbox.CornerPoint(4).z);

	glVertex3f(bbox.CornerPoint(5).x, bbox.CornerPoint(5).y, bbox.CornerPoint(5).z);
	glVertex3f(bbox.CornerPoint(1).x, bbox.CornerPoint(1).y, bbox.CornerPoint(1).z);

	glVertex3f(bbox.CornerPoint(5).x, bbox.CornerPoint(5).y, bbox.CornerPoint(5).z);
	glVertex3f(bbox.CornerPoint(4).x, bbox.CornerPoint(4).y, bbox.CornerPoint(4).z);

	glVertex3f(bbox.CornerPoint(5).x, bbox.CornerPoint(5).y, bbox.CornerPoint(5).z);
	glVertex3f(bbox.CornerPoint(7).x, bbox.CornerPoint(7).y, bbox.CornerPoint(7).z);

	glVertex3f(bbox.CornerPoint(3).x, bbox.CornerPoint(3).y, bbox.CornerPoint(3).z);
	glVertex3f(bbox.CornerPoint(7).x, bbox.CornerPoint(7).y, bbox.CornerPoint(7).z);

	glVertex3f(bbox.CornerPoint(3).x, bbox.CornerPoint(3).y, bbox.CornerPoint(3).z);
	glVertex3f(bbox.CornerPoint(1).x, bbox.CornerPoint(1).y, bbox.CornerPoint(1).z);

	glVertex3f(bbox.CornerPoint(3).x, bbox.CornerPoint(3).y, bbox.CornerPoint(3).z);
	glVertex3f(bbox.CornerPoint(2).x, bbox.CornerPoint(2).y, bbox.CornerPoint(2).z);

	glVertex3f(bbox.CornerPoint(6).x, bbox.CornerPoint(6).y, bbox.CornerPoint(6).z);
	glVertex3f(bbox.CornerPoint(7).x, bbox.CornerPoint(7).y, bbox.CornerPoint(7).z);

	glVertex3f(bbox.CornerPoint(6).x, bbox.CornerPoint(6).y, bbox.CornerPoint(6).z);
	glVertex3f(bbox.CornerPoint(4).x, bbox.CornerPoint(4).y, bbox.CornerPoint(4).z);

	glVertex3f(bbox.CornerPoint(6).x, bbox.CornerPoint(6).y, bbox.CornerPoint(6).z);
	glVertex3f(bbox.CornerPoint(2).x, bbox.CornerPoint(2).y, bbox.CornerPoint(2).z);

	glEnd();
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
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
	static_assert(Component::Type::UNKNOWN == 5, "Update factory code");

	const Component* existing_component = GetComponent(type);

	Component* ret = nullptr;

	switch (type)
	{
	case Component::TRANSFORM:
		if (existing_component != nullptr)
		{
			if (existing_component != transform)
			{
				LOG("Error in transform: Transform pointer different from transform component %s", name.c_str());
			}
			else
			{
				LOG("Error adding component: Already a transform in %s", name.c_str());
			}
		}
		else
		{
			ret = new ComponentTransform(this);
			transform = (ComponentTransform*)ret;
		}
		break;
	case Component::MESH:
		if (existing_component != nullptr)
		{
			LOG("Error adding component: Already a mesh in %s", name.c_str());
		}
		else
		{
			ret = new ComponentMesh(this);
			mesh = (ComponentMesh*)ret;
		}
		break;
	case Component::MATERIAL:
		if (existing_component != nullptr)
		{
			LOG("Error adding component: Already a material in %c", name.c_str());
		}
		else
		{
			ret = new ComponentMaterial(this);
			material = (ComponentMaterial*)ret;
		}
		break;
	case Component::CAMERA:
		ret = new ComponentCamera(this);
		break;
	case Component::ANIMATION:
		ret = new ComponentAnim(this);
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

	for (std::vector<Component*>::const_iterator it = components.cbegin(); it != components.cend(); ++it)
	{
		if ((*it)->GetType() == type && (*it)->IsActive())
			ret = *it;
	}

	return ret;
}

const std::vector<Component*> GameObject::GetComponents(Component::Type type) const
{
	std::vector<Component*> ret;
	for (std::vector<Component*>::const_iterator it = components.cbegin(); it != components.cend(); ++it)
	{
		if ((*it)->GetType() == type && (*it)->IsActive())
			ret.push_back(*it);
	}
	return ret;
}

const GameObject* GameObject::FindByName(const std::string& name) const
{
	const GameObject* ret = nullptr;

	for (std::vector<GameObject*>::const_iterator it = childs.cbegin(); it != childs.cend(); it++)
	{
		if ((*it)->name == name)
			ret = (*it);
		else if (ret == nullptr)
			ret = (*it)->FindByName(name);
	}

	return ret;
}

void GameObject::SetLocalTransform(const float3& position, const float3& scaling, const Quat& rotation)
{
	if (transform == nullptr)
		transform = (ComponentTransform*)CreateComponent(Component::Type::TRANSFORM);

	transform->Load(position, scaling, rotation);
}

void GameObject::SetLocalTransformNoScale(const float3& position, const Quat& rotation)
{
	if (transform == nullptr)
		SetLocalTransform(position, float3::one, rotation);

	transform->Load(position, rotation);
}

void GameObject::LoadMeshFromScene(aiMesh* scene_mesh, const aiScene* scene, const aiString& folder_path)
{
	ComponentMesh* mesh = (ComponentMesh*)CreateComponent(Component::Type::MESH);
	mesh->Load(scene_mesh);
	mesh->folder_path = folder_path;

	ComponentMaterial* material = (ComponentMaterial*)CreateComponent(Component::Type::MATERIAL);
	aiMaterial* scene_material = scene->mMaterials[scene_mesh->mMaterialIndex];
	material->Load(scene_material, folder_path);
}

void GameObject::LoadAnim(const char * name, const char * file)
{
	App->animations->Load(name, file);
	ComponentAnim* anim = (ComponentAnim*)CreateComponent(Component::Type::ANIMATION);
	anim->SetName(name);
	anim->Play(true);
}

void GameObject::LoadBones()
{
	if (mesh != nullptr)
		mesh->LoadBones();

	for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
		(*it)->LoadBones();
}

void GameObject::ChangeAnim(const char* name, unsigned int duration)
{
	const Component* component_anim = GetComponent(Component::Type::ANIMATION);
	((ComponentAnim*)component_anim)->BlendTo(name, duration);
}

void GameObject::CalculateGlobalTransformMatrix(float4x4& global_transform) const
{
	if (parent != nullptr)
	{
		float4x4 parent_transform = float4x4::identity;
		parent->CalculateGlobalTransformMatrix(parent_transform);
		global_transform = global_transform * parent_transform;
	}
	if (transform != nullptr)
	{
		global_transform = global_transform * transform->GetLocalTransformMatrix();
	}
}

void GameObject::CalculateBoneGlobalTransformMatrix(float4x4& global_transform) const
{
	if (parent != nullptr && parent != root)
	{
		float4x4 parent_transform = float4x4::identity;
		parent->CalculateBoneGlobalTransformMatrix(parent_transform);
		global_transform = global_transform * parent_transform;
	}
	if (transform != nullptr)
	{
		global_transform = global_transform * transform->GetLocalTransformMatrix();
	}
}

const float4x4& GameObject::GetLocalTransformMatrix() const
{
	return transform->GetLocalTransformMatrix();
}
