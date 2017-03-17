#ifndef MODULEANIMATION_H
#define MODULEANIMATION_H

#include "Module.h"
#include <map>
#include <vector>
#include <assimp/types.h>
#include "Math.h"

#define MODULE_ANIMATION "ModuleAnimation"

class Timer;

struct LessString
{
	bool operator()(const aiString& left, const aiString& right) const
	{
		return ::strcmp(left.data, right.data) < 0;
	}
};
struct NodeAnim
{
	aiString name;
	float3* positions = nullptr;
	Quat* rotations = nullptr;
	unsigned int num_positions = 0;
	unsigned int num_rotations = 0;
};

typedef std::map<aiString, NodeAnim*, LessString> NodeAnimMap;

struct Anim
{
	unsigned int duration = 0;
	unsigned int num_channels = 0;
	NodeAnimMap channels;
};

struct AnimInstance
{
	Anim* anim;
	unsigned int time = 0;
	bool loop = true;

	AnimInstance* next = nullptr;
	unsigned int blend_duration = 0;
	unsigned int blend_time = 0;
};

class ModuleAnimations : public Module
{

	typedef std::map<aiString, Anim*, LessString> AnimMap;
	typedef std::vector<AnimInstance*> InstanceList;
	typedef std::vector<unsigned int> HoleList;

public:
	ModuleAnimations();
	~ModuleAnimations();

	update_status Update(float dt);
	bool CleanUp();
	
	void Load(const char* name, const char* file);
	unsigned int Play(const char* name, bool loop = false);
	void Stop(unsigned int id);
	void BlendTo(unsigned int id, const char* name, unsigned int blend_time);

	bool GetTransform(unsigned int id, const char* channel, float3& position, Quat& rotation) const;

private:
	float3 InterpFloat3(const float3& first, const float3& second, float lambda) const;
	Quat InterpQuaternion(const Quat& first, const Quat& second, float lambda) const;

private:
	AnimMap animations;
	InstanceList instances;
	HoleList holes;
	unsigned int anim_next_id = 0;
	Timer* timer;
};

#endif // !MODULEANIMATION_H

