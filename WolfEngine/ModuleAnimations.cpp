#include "ModuleAnimations.h"
#include "Timer.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/anim.h>


ModuleAnimations::ModuleAnimations() : Module(MODULE_ANIMATION)
{
	timer = new Timer();
	timer->Start();
}

ModuleAnimations::~ModuleAnimations()
{
	CleanUp();
}

update_status ModuleAnimations::Update(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleAnimations::CleanUp()
{
	for (AnimMap::iterator it = animations.begin(); it != animations.end(); ++it)
	{
		for (int i = 0; i < it->second->num_channels; i++)
		{
			RELEASE_ARRAY(it->second->channels[i]->positions);
			RELEASE_ARRAY(it->second->channels[i]->rotations);
			RELEASE(it->second->channels[i]);
		}
		RELEASE_ARRAY(it->second->channels);
		RELEASE(it->second);
	}
	animations.clear();
	RELEASE(timer);
	return true;
}

void ModuleAnimations::Load(const char * name, const char * file)
{
	aiString animation_name = aiString();
	animation_name.Append(name);

	aiString file_path = aiString();
	file_path.Append(file);

	const aiScene* scene = aiImportFile(file_path.data, aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr)
	{
		if (scene->HasAnimations())
		{
			for (unsigned int i = 0; i <= scene->mNumAnimations; ++i)
			{
				aiAnimation* scene_animation = scene->mAnimations[i];
				Anim* anim = new Anim();
				double ticks_per_miliseconds = scene_animation->mTicksPerSecond / 1000;
				anim->duration = (unsigned int)scene_animation->mDuration / ticks_per_miliseconds;
				anim->num_channels = scene_animation->mNumChannels;
				anim->channels = new NodeAnim*[anim->num_channels];
				for (unsigned int j = 0; j < anim->num_channels; ++j)
				{

					aiNodeAnim* scene_nodeanim = scene_animation->mChannels[j];
					NodeAnim* node_anim = new NodeAnim();
					node_anim->name = scene_nodeanim->mNodeName.data;
					node_anim->num_positions = scene_nodeanim->mNumPositionKeys;
					node_anim->positions = new float3[node_anim->num_positions];
					for (unsigned int k = 0; k < node_anim->num_positions; ++k)
					{
						aiVector3D position_aux = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue;
						node_anim->positions[k] = { position_aux.x, position_aux.y, position_aux.z };
					}
					node_anim->num_rotations = scene_nodeanim->mNumRotationKeys;
					node_anim->rotations = new Quat[node_anim->num_rotations];
					for (unsigned int k = 0; k < node_anim->num_rotations; ++k) 
					{
						aiQuaternion rotation_aux = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue;
						node_anim->rotations[k] = { rotation_aux.x, rotation_aux.y, rotation_aux.z, rotation_aux.w };
					}
						
					anim->channels[j] = node_anim;
				}
				animations[animation_name] = anim;
			}
		}
	}
	else
	{
		LOG("Error loading animation: No animation found in path %s", file_path.data);
	}
	
	aiReleaseImport(scene);
}

unsigned int ModuleAnimations::Play(const char * name, bool loop)
{
	unsigned int id = 0;
	aiString animation_name = aiString();
	animation_name.Append(name);
	AnimMap::iterator it = animations.find(animation_name);
	if (it != animations.end())
	{
		AnimInstance* anim_instance = new AnimInstance();
		anim_instance->anim = it->second;
		anim_instance->time = timer->GetTimeInMs();
		anim_instance->loop = loop;
		if (!holes.empty()) {
			id = holes.back();
			holes.pop_back();
			instances[id] = anim_instance;
		}
		else {
			id = anim_next_id++;
			instances.push_back(anim_instance);
		}
	}
	return id;
}

void ModuleAnimations::Stop(unsigned int id)
{
	if (instances[id] != nullptr)
	{
		instances[id] = nullptr;
		holes.push_back(id);
	}
}

void ModuleAnimations::BlendTo(unsigned int id, const char * name, unsigned int blend_time)
{
}

bool ModuleAnimations::GetTransform(unsigned int id, const char * channel, float3 & positon, Quat & rotation) const
{
	return false;
}

float3 ModuleAnimations::InterpVector3(const float3 & first, const float3 & second, float lambda) const
{
	return first * (1.0f - lambda) + second * lambda;
}

Quat ModuleAnimations::InterpQuaternion(const Quat & first, const Quat & second, float lambda) const
{
	Quat result;
	float dot = first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w;

	if (dot >= 0.0f)
	{
		result.x = first.x * (1.0f - lambda) + second.x * lambda;
		result.y = first.y * (1.0f - lambda) + second.y * lambda;
		result.z = first.z * (1.0f - lambda) + second.z * lambda;
		result.w = first.w * (1.0f - lambda) + second.w * lambda;
	}
	else
	{
		result.x = first.x * (1.0f - lambda) + second.x * -lambda;
		result.y = first.y * (1.0f - lambda) + second.y * -lambda;
		result.z = first.z * (1.0f - lambda) + second.z * -lambda;
		result.w = first.w * (1.0f - lambda) + second.w * -lambda;
	}

	return result;
}
