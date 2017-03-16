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
	return update_status();
}

bool ModuleAnimations::CleanUp()
{
	return false;
}

void ModuleAnimations::Load(const char * name, const char * file)
{
	aiString file_path = aiString();
	file_path.Append(file);

	const aiScene* scene = aiImportFile(file_path.data, aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene->HasAnimations()) 
	{
		for (unsigned int i = 0; i <= scene->mNumAnimations; ++i)
		{
			Anim* anim = new Anim();
			double ticks_per_miliseconds = scene->mAnimations[i]->mTicksPerSecond / 1000;
			anim->duration = (unsigned int)scene->mAnimations[i]->mDuration / ticks_per_miliseconds;
			anim->num_channels = scene->mAnimations[i]->mNumChannels;
			anim->channels = new NodeAnim[scene->mAnimations[i]->mNumChannels];
			for (unsigned int j = 0; i <= scene->mAnimations[i]->mNumChannels; ++i) 
			{
				NodeAnim node_anim = NodeAnim();
				node_anim.name = scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();
				node_anim.num_positions = scene->mAnimations[i]->mChannels[j]->mNumPositionKeys;
				node_anim.num_rotations = scene->mAnimations[i]->mChannels[j]->mNumRotationKeys;
				node_anim.positions = new float3[scene->mAnimations[i]->mChannels[j]->mNumPositionKeys];
				for (unsigned int k = 0; k <= scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; ++k)
				{
					aiVector3D position_aux = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue;
					node_anim.positions[k] = { position_aux.x, position_aux.y, position_aux.z };
				}
				node_anim.rotations = new Quat[scene->mAnimations[i]->mChannels[j]->mNumRotationKeys];
				for (unsigned int k = 0; k <= scene->mAnimations[i]->mChannels[j]->mNumRotationKeys; ++k)
				{
					aiQuaternion rotation_aux = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue;
					node_anim.rotations[k] = { rotation_aux.x, rotation_aux.y, rotation_aux.z, rotation_aux.w };
				}
				anim->channels[j] = node_anim;
			}
			animations[scene->mAnimations[i]->mName.C_Str()] = anim;
		}
	}
}

unsigned int ModuleAnimations::Play(const char * name)
{
	return 0;
}

void ModuleAnimations::Stop(unsigned int id)
{
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
