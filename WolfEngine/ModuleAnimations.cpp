#include "ModuleAnimations.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/anim.h>


ModuleAnimations::ModuleAnimations() : Module(MODULE_ANIMATION)
{
}

ModuleAnimations::~ModuleAnimations()
{
}

update_status ModuleAnimations::Update(float dt)
{
	for (InstanceList::iterator it = instances.begin(); it != instances.end(); ++it)
	{
		if (*it != nullptr)
		{
			(*it)->time += 1000*dt;
		}
	}
	return UPDATE_CONTINUE;
}

bool ModuleAnimations::CleanUp()
{
	for (AnimMap::iterator it = animations.begin(); it != animations.end(); ++it)
	{
		for (NodeAnimMap::iterator it2 = it->second->channels.begin(); it2 != it->second->channels.end(); ++it2)
		{
			RELEASE_ARRAY(it2->second->positions);
			RELEASE_ARRAY(it2->second->rotations);
			RELEASE(it2->second);
		}
		it->second->channels.clear();
		RELEASE(it->second);
	}
	for (InstanceList::iterator it = instances.begin(); it != instances.end(); ++it)
	{
		RELEASE(*it);
	}
	animations.clear();
	instances.clear();
	holes.clear();
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
			for (unsigned int i = 0; i < scene->mNumAnimations; ++i)
			{
				aiAnimation* scene_animation = scene->mAnimations[i];
				Anim* anim = new Anim();
				double ticks_per_miliseconds = scene_animation->mTicksPerSecond / 1000;
				anim->duration = (unsigned int)scene_animation->mDuration / ticks_per_miliseconds;
				anim->num_channels = scene_animation->mNumChannels;
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
						
					anim->channels[node_anim->name] = node_anim;
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
		anim_instance->time = 0;
		anim_instance->loop = loop;
		if (!holes.empty()) 
		{
			id = holes.back();
			holes.pop_back();
			instances[id] = anim_instance;
		}
		else 
		{
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

bool ModuleAnimations::GetTransform(unsigned int id, const char * channel, float3 & position, Quat & rotation) const
{
	bool res = true;
	AnimInstance* instance = instances[id];
	Anim* animation = instance->anim;
	aiString channel_name = aiString();
	channel_name.Append(channel);
	NodeAnimMap::iterator it = animation->channels.find(channel_name);
	NodeAnim* node = (it != animation->channels.end()) ? animation->channels[channel_name] : nullptr;
	if (res = (node != nullptr && instance != nullptr)) 
	{
		if(!instance->loop && (instance->time >= animation->duration))
		{
			position = node->positions[node->num_positions - 1];
			rotation = node->rotations[node->num_rotations - 1];
		}
		else
		{
			float pos_key = float(instance->time * (node->num_positions - 1)) / float(animation->duration);
			float rot_key = float(instance->time * (node->num_rotations - 1)) / float(animation->duration);

			unsigned int pos_index = unsigned(pos_key);
			unsigned int rot_index = unsigned(rot_key);
			unsigned int pos_index_sec = (pos_index + 1) % node->num_positions;
			unsigned int rot_index_sec = (rot_index + 1) % node->num_rotations;

			float pos_lambda = pos_key - float(pos_index);
			float rot_lambda = rot_key - float(rot_index);
			
			pos_index = pos_index % node->num_positions;
			rot_index = rot_index % node->num_rotations;

			position = InterpFloat3(node->positions[pos_index], node->positions[pos_index_sec], pos_lambda);
			rotation = InterpQuaternion(node->rotations[rot_index], node->rotations[rot_index_sec], rot_lambda);
		}
	}
	
	return res;
}

float3& ModuleAnimations::InterpFloat3(const float3& first, const float3& second, float lambda) const
{
	return first * (1.0f - lambda) + second * lambda;
}

Quat& ModuleAnimations::InterpQuaternion(const Quat& first, const Quat& second, float lambda) const
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
	result.Normalize();

	return result;
}
