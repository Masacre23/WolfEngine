#include "ComponentAnim.h"
#include "Application.h"
#include "ModuleAnimations.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "Interface.h"
#include <vector>
#include <assimp/scene.h>


ComponentAnim::ComponentAnim(GameObject * parent) : Component(Component::Type::ANIMATION, parent)
{
}

ComponentAnim::~ComponentAnim()
{
}

bool ComponentAnim::OnEditor()
{
	if (ImGui::CollapsingHeader("Animator"))
	{
		ImGui::Checkbox("Show bones", &draw_bones);

		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);
		ImGui::SliderInt("Blend Time", &blend_time, 10, 1000);

		ImGui::TextWrapped("Current animation:");
		ImGui::TextWrapped("%s", current_animation.data);

		if (anim_id != -1)
		{
			if (ImGui::Button("Stop"))
				StopCurrent();
		}
		else
		{
			if (ImGui::Button("Play"))
				PlayCurrent(true);
		}

		for (std::list<aiString>::iterator it = animations.begin(); it != animations.end(); ++it)
		{
			if (ImGui::Button(it->data))
				PlayAnimation(it->data, true);
		}
	}

	return ImGui::IsItemClicked();
}

void ComponentAnim::SaveComponent()
{
	backed_animation = current_animation;
	backed_blend_time = blend_time;
	backed_is_playing = IsPlaying();
}

void ComponentAnim::RestoreComponent()
{
	blend_time = backed_blend_time;
	current_animation = backed_animation;
	StopCurrent();
	if (backed_is_playing)
		PlayCurrent(true);
}

bool ComponentAnim::OnAnimationUpdate()
{
	BROFILER_CATEGORY("ComponentAnimation-OnAnimationUpdate", Profiler::Color::Aquamarine);

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
				node->SetLocalTransform(position, rotation);
			}
		}
	}
	return true;
}

void ComponentAnim::LoadAnimations(const char* animation)
{
	aiString new_animation = aiString();
	new_animation.Append(animation);
	this->animations.push_back(new_animation);

	current_animation = *(animations.begin());
}

void ComponentAnim::LoadAnimations(const std::list<std::string>& animations)
{
	for (std::list<std::string>::const_iterator it = animations.cbegin(); it != animations.end(); ++it)
	{
		aiString new_animation = aiString();
		new_animation.Append(it->c_str());
		this->animations.push_back(new_animation);
	}

	current_animation = *(this->animations.begin());
}

void ComponentAnim::SetName(const char * name)
{
	current_animation = aiString();
	current_animation.Append(name);
}

void ComponentAnim::PlayAnimation(const char* name, bool loop)
{
	if (anim_id == -1)
	{
		SetName(name);
		PlayCurrent(loop);
	}
	else
	{
		BlendTo(name, blend_time);
	}
}

void ComponentAnim::PlayCurrent(bool loop)
{
	anim_id = App->animations->Play(current_animation.data, loop);
}

void ComponentAnim::StopCurrent()
{
	if (anim_id != -1)
		App->animations->Stop(anim_id);
	anim_id = -1;
}

bool ComponentAnim::IsPlaying() const
{
	return anim_id != -1;
}

void ComponentAnim::BlendTo(const char* name, unsigned int duration)
{
	current_animation = aiString();
	current_animation.Append(name);

	App->animations->BlendTo(anim_id, name, duration);
}

