#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Application.h"
#include "ModuleLevel.h"
#include "OpenGL.h"

GameObject::GameObject(GameObject* parent, const std::string& name) : name(name)
{
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
	return true;
}

void GameObject::Draw() const
{
	glPushMatrix();

	if(selected)
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
	for (std::vector<GameObject*>::const_iterator it = childs.begin(); it != childs.end(); ++it)
		(*it)->RecursiveDrawHierarchy(float4x4::identity);
}



void GameObject::RecursiveDrawHierarchy(const float4x4& parent_transform) const
{
	float4x4 this_transform = float4x4::identity;
	if (transform != nullptr)
		this_transform = ((ComponentTransform*)transform)->GetTransform();
	if (parent != App->level->GetRoot())
	{
		this_transform.Mul(parent_transform);

		glLineWidth(1.0f);
		glDepthRange(0.0, 0.1);
		glColor3f(0.0, 0.0f, 3.0f);
		glBegin(GL_LINES);

		float3 translate;
		Quat rotation;
		float3 scale;
		this_transform.Decompose(translate, rotation, scale);

		glVertex3f(translate.x, translate.y, translate.z);

		translate;
		rotation;
		scale;
		parent_transform.Decompose(translate, rotation, scale);
		glVertex3f(translate.x, translate.y, translate.z);

		glEnd();
		glColor3f(0.0, 0.0f, 0.0f);
		glDepthRange(0.1, 0.9);
	}

	for (std::vector<GameObject*>::const_iterator it = childs.begin(); it != childs.end(); ++it)
		(*it)->RecursiveDrawHierarchy(this_transform);
	
}

void GameObject::DrawAABBBox() const {
	//Draw AABB box.
	glLineWidth(2.0f);
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
			transform = ret;
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
			mesh = ret;
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
			material = ret;
		}
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
