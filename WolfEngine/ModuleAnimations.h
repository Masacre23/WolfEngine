#ifndef MODULEANIMATION_H
#define MODULEANIMATION_H

#include "Module.h"
#include <string>

#define MODULE_ANIMATION "ModuleAnimation"

struct float3;
struct Quat;

struct NodeAnim
{
	std::string name;
	float3* positions = nullptr;
	Quat* rotations = nullptr;
	unsigned int num_positions = 0;
	unsigned int num_rotations = 0;
};

struct Anim
{
	unsigned int duration = 0;
	unsigned int num_channels = 0;
	NodeAnim* channels = nullptr;
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

public:
	ModuleAnimations();
	~ModuleAnimations();

};

#endif // !MODULEANIMATION_H

