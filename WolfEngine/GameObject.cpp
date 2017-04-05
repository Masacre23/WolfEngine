#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ComponentAnim.h"
#include "ComponentBillboard.h"
#include "ComponentParticleSystem.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleAnimations.h"
#include "ModuleLevel.h"
#include "ModuleRender.h"
#include "ModuleTimeController.h"
#include "ModuleTextures.h"
#include "OpenGL.h"
#include "Color.h"
#include "Primitive.h"

GameObject::GameObject(GameObject* parent, GameObject* root_object, const std::string& name) : name(name), root(root_object)
{
	if (root_object == nullptr)
		root = this;

	SetParent(parent);
	components.push_back(transform = new ComponentTransform(this));

	//Init BoundingBox (in case some GameObjects don't have a MeshComponent)
	initial_bbox.SetNegativeInfinity();
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
	if (App->camera->InsideCulling(bbox))
	{
		for (std::vector<Component*>::const_iterator it = components.begin(); it != components.cend(); ++it)
			if ((*it)->IsActive())
				(*it)->OnUpdate();
		for (std::vector<GameObject*>::const_iterator it = childs.begin(); it != childs.end(); ++it)
			if ((*it)->IsActive())
				(*it)->Update();
	}
	return true;
}

void GameObject::Draw() const
{
	if (App->camera->InsideCulling(bbox))
	{
		glPushMatrix();

		if (selected)
		{
			App->renderer->DrawBoundingBox(bbox, Colors::Green);
			App->renderer->DrawBoundingBox(transform_bbox, Colors::Blue);
		}

		if (transform != nullptr)
			if (transform->IsActive())
				transform->OnDraw();

		if (selected)
			App->renderer->DrawAxis();

		glBindTexture(GL_TEXTURE_2D, 0);

		App->renderer->DrawColor(Colors::White);

		const Component* material = GetComponent(Component::MATERIAL);
		bool material_on = false;
		if (material != nullptr)
		{
			if (material->IsActive())
			{
				material->OnDraw();
				material_on = true;
			}	
			else
				glBindTexture(GL_TEXTURE_2D, App->textures->texture_checkers);	
		}
			
		ComponentMesh* mesh = (ComponentMesh*) GetComponent(Component::MESH);
		if (mesh != nullptr)
		{
			mesh->SetUseNormals(material_on);
			if (mesh->IsActive())
				mesh->OnDraw();
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		glPopMatrix();

		const Component* camera = GetComponent(Component::Type::CAMERA);
		if (camera != nullptr)
			if (camera->IsActive())
				camera->OnDraw();

		if (billboard != nullptr)
			if (billboard->IsActive())
				billboard->OnDraw();

		if(particle_system != nullptr)
			if (particle_system->IsActive())
				particle_system->OnDraw();

		for (std::vector<GameObject*>::const_iterator it = childs.begin(); it != childs.end(); ++it)
			if ((*it)->IsActive())
				(*it)->Draw();

	}
}

void GameObject::DrawHierarchy() const
{
	glDepthRange(0.0, 0.01);
	glLineWidth(2.0f);
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	
	App->renderer->DrawColor(Colors::Blue);

	glPushMatrix();

	if (transform != nullptr)
		if (transform->IsActive())
			transform->OnDraw();

	for (std::vector<GameObject*>::const_iterator it = childs.cbegin(); it != childs.cend(); ++it)
	{
		if ((*it)->IsActive() && (*it)->is_bone)
		{
			float3 child_transform = (*it)->transform->GetPosition();
			glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(child_transform.x, child_transform.y, child_transform.z);
			glEnd();
		}	
	}

	glPopMatrix();

	for (std::vector<GameObject*>::const_iterator it = childs.cbegin(); it != childs.cend(); ++it)
		if ((*it)->IsActive())
			(*it)->RecursiveDrawHierarchy();

	App->renderer->DrawColor(Colors::Black);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glDepthRange(0.01, 1.0);
}

void GameObject::RecursiveDrawHierarchy() const
{
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
		}
	}

	glPopMatrix();	

	for (std::vector<GameObject*>::const_iterator it = childs.cbegin(); it != childs.cend(); ++it)
		if ((*it)->IsActive())
			(*it)->RecursiveDrawHierarchy();
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
			APPLOG("Error detected: GameObject has a parent but it isn't in parent's childs' vector.")
		}
	}

	this->parent = parent;
	parent->childs.push_back(this);
}

Component* GameObject::CreateComponent(Component::Type type)
{
	static_assert(Component::Type::UNKNOWN == 7, "Update factory code");

	const Component* existing_component = GetComponent(type);

	Component* ret = nullptr; 

	switch (type)
	{
	case Component::TRANSFORM:
		if (existing_component != nullptr)
		{
			if (existing_component != transform)
			{
				APPLOG("Error in transform: Transform pointer different from transform component %s", name.c_str());
			}
			else
			{
				APPLOG("Error adding component: Already a transform in %s", name.c_str());
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
			APPLOG("Error adding component: Already a mesh in %s", name.c_str());
		}
		else
		{
			ret = new ComponentMesh(this);
		}
		break;
	case Component::MATERIAL:
		if (existing_component != nullptr)
		{
			APPLOG("Error adding component: Already a material in %c", name.c_str());
		}
		else
		{
			ret = new ComponentMaterial(this);
		}
		break;
	case Component::CAMERA:
		ret = new ComponentCamera(this);
		break;
	case Component::ANIMATION:
		ret = new ComponentAnim(this);
		break;
	case Component::BILLBOARD:
		ret = new ComponentBillboard(this, 1, 1);
		ret->Enable();
		billboard = (ComponentBillboard*)ret;
		break;
	case Component::PARTICLE:
		ret = new ComponentParticleSystem(this);
		particle_system = (ComponentParticleSystem*)ret;
		particle_system->Init(10, aiVector2D(0, 0), 2, 2, "Resources/rainSprite.tga", aiVector2D(0,0));
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

void GameObject::DeleteComponent(Component* component)
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if (*it == component)
		{
			RELEASE(*it);
			components.erase(it);
			break;
		}
	}
}

const Component* GameObject::GetComponent(Component::Type type, bool only_active) const
{
	Component* ret = nullptr;

	for (std::vector<Component*>::const_iterator it = components.cbegin(); it != components.cend(); ++it)
	{
		if ((*it)->GetType() == type)
			ret = *it;	
	}

	if (only_active && !ret->IsActive())
	{
		ret = nullptr;
		for (std::vector<Component*>::const_iterator it = components.cbegin(); it != components.cend(); ++it)
		{
			if ((*it)->GetType() == type && (*it)->IsActive())
				ret = *it;
		}
	}

	return ret;
}

Component* GameObject::GetComponent(Component::Type type, bool only_active)
{
	Component* ret = nullptr;

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->GetType() == type)
			ret = *it;
	}

	if (only_active && !ret->IsActive())
	{
		ret = nullptr;
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
		{
			if ((*it)->GetType() == type && (*it)->IsActive())
				ret = *it;
		}
	}

	return ret;
}

GameObject* GameObject::FindByName(const std::string& name) const
{
	GameObject* ret = nullptr;

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

void GameObject::SetLocalTransform(const float3& position, const Quat& rotation)
{
	if (transform == nullptr)
		SetLocalTransform(position, float3::one, rotation);

	transform->Load(position, rotation);
}

void GameObject::SetLocalTransform(const float3 & position)
{
	if (transform == nullptr)
		SetLocalTransform(position, float3::one, Quat::identity);

	transform->Load(position);
}

void GameObject::LoadMesh(aiMesh* scene_mesh, const aiScene* scene, const aiString& folder_path)
{
	ComponentMesh* mesh = (ComponentMesh*)CreateComponent(Component::Type::MESH);
	mesh->Load(scene_mesh);
	mesh->folder_path = folder_path;
}

void GameObject::LoadMesh(Primitive* primitive)
{
	ComponentMesh* mesh = (ComponentMesh*) CreateComponent(Component::MESH);
	mesh->Load(primitive);
}

void GameObject::LoadMaterial(aiMesh* scene_mesh, const aiScene* scene, const aiString& folder_path)
{
	ComponentMaterial* material = (ComponentMaterial*)CreateComponent(Component::Type::MATERIAL);
	aiMaterial* scene_material = scene->mMaterials[scene_mesh->mMaterialIndex];
	material->Load(scene_material, folder_path);
}

void GameObject::LoadMaterial(const aiString& path)
{
	ComponentMaterial* material = (ComponentMaterial*)CreateComponent(Component::Type::MATERIAL);
	material->LoadTexture(path);
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
	ComponentMesh* mesh = (ComponentMesh*) GetComponent(Component::MESH);
	if (mesh != nullptr)
		mesh->LoadBones();

	for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
		(*it)->LoadBones();
}

void GameObject::SetAABB(AABB box)
{
	initial_bbox = box;
	transform_bbox.SetFrom(initial_bbox, GetGlobalTransformMatrix());
	bbox.SetFrom(transform_bbox);
}

void GameObject::ChangeAnim(const char* name, unsigned int duration)
{
	const Component* component_anim = GetComponent(Component::Type::ANIMATION);
	((ComponentAnim*)component_anim)->BlendTo(name, duration);
}

void GameObject::RecursiveUpdateTransforms(const float4x4& parent)
{
	float4x4 global = transform->UpdateGlobalTransform(parent);

	for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
		(*it)->RecursiveUpdateTransforms(global);
}

void GameObject::RecursiveUpdateBoundingBox(bool force_recalculation)
{
	bool child_recalc = false;

	if (transform->transform_change || force_recalculation)
	{
		transform_bbox.SetFrom(initial_bbox, GetGlobalTransformMatrix());
		bbox.SetFrom(transform_bbox);
		transform->transform_change = false;
		child_recalc = true;
	}

	for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
		(*it)->RecursiveUpdateBoundingBox(child_recalc);
}

void GameObject::RecursiveSaveLocalTransform()
{
	transform->SaveTransform();

	for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
		(*it)->RecursiveSaveLocalTransform();
}

void GameObject::RecursiveLoadLocalTransform()
{
	transform->LoadTransform();

	for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
		(*it)->RecursiveSaveLocalTransform();
}

const float4x4& GameObject::GetLocalTransformMatrix() const
{
	return transform->GetLocalTransformMatrix();
}

const float4x4 & GameObject::GetGlobalTransformMatrix() const
{
	return transform->GetGlobalTransformMatrix();
}
