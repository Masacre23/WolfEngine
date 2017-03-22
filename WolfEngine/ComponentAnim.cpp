#include "ComponentAnim.h"
#include "Application.h"
#include "ModuleAnimations.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include <vector>
#include <assimp/scene.h>


ComponentAnim::ComponentAnim(GameObject * parent) : Component(Component::Type::ANIMATION, parent)
{
}

ComponentAnim::~ComponentAnim()
{
}

bool ComponentAnim::OnUpdate()
{
	
	if (anim_id != -1) 
	{
		std::vector<GameObject*> nodes;
		nodes.push_back(parent);
		while (nodes.size() != 0) 
		{
			GameObject* node = nodes.back();
			nodes.pop_back();
			for (std::vector<GameObject*>::const_iterator it = node->childs.begin(); it != node->childs.end(); ++it)
				nodes.push_back((*it));
			float3 position;
			Quat rotation;
			bool exits = App->animations->GetTransform(anim_id, node->name.c_str(), position, rotation);
			if (exits)
			{
				node->SetLocalTransformNoScale(position, rotation);
			}
		}
	}
	return true;
}

bool ComponentAnim::OnEditor(int selection_mask, int id)
{
	return false;
}

void ComponentAnim::SetName(const char * name)
{
	anim_name = aiString();
	anim_name.Append(name);
}

void ComponentAnim::Play(bool loop)
{
	anim_id = App->animations->Play(anim_name.C_Str(), loop);
}

void ComponentAnim::Stop()
{
	if(anim_id != -1)
		App->animations->Stop(anim_id);
	anim_id = -1;
}

bool ComponentAnim::IsPlaying()
{
	return anim_id != -1;
}

void ComponentAnim::BlendTo(const char * name, unsigned int duration)
{
	App->animations->BlendTo(anim_id, name, duration);
}

