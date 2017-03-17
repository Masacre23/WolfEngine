#ifndef MODULEANIMATION_H
#define MODULEANIMATION_H

#include "Module.h"
#include <map>
#include <vector>
#include <assimp/types.h>
#include "Math.h"

#define MODULE_ANIMATION "ModuleAnimation"

struct NodeAnim
{
	aiString name;
	aiVector3D* positions = nullptr;
	aiQuaternion* rotations = nullptr;
	unsigned int num_positions = 0;
	unsigned int num_rotations = 0;
};

struct Anim
{
	unsigned int duration = 0;
	unsigned int num_channels = 0;
	NodeAnim** channels = nullptr;
};

struct AnimInstance
{
	unsigned int anim;
	unsigned int time = 0;
	bool loop = true;

	AnimInstance* next = nullptr;
	unsigned int blend_duration = 0;
	unsigned int blend_time = 0;
};

class ModuleAnimations : public Module
{
	struct LessString
	{
		bool operator()(const aiString& left, const aiString& right) const
		{
			return ::strcmp(left.data, right.data) < 0;
		}
	};

	typedef std::map<aiString, Anim*, LessString> AnimMap;
	typedef std::vector<AnimInstance*> InstanceList;
	typedef std::vector<unsigned int> HoleList;

	AnimMap animations;
	InstanceList intances;
	HoleList holes;

public:
	ModuleAnimations();
	~ModuleAnimations();

	update_status Update(float dt);
	bool CleanUp();
	
	void Load(const char* name, const char* file);
	unsigned int Play(const char* name);
	void Stop(unsigned int id);
	void BlendTo(unsigned int id, const char* name, unsigned int blend_time);

	bool GetTransform(unsigned int id, const char* channel, float3& positon, Quat& rotation) const;

private:
	float3 InterpVector3(const float3& first, const float3& second, float lambda) const;
	Quat InterpQuaternion(const Quat& first, const Quat& second, float lambda) const;

};

#endif // !MODULEANIMATION_H

