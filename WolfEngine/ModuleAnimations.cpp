#include "ModuleAnimations.h"



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
	return float3();
}

Quat ModuleAnimations::InterpQuaternion(const Quat & first, const Quat & second, float lambda) const
{
	return Quat();
}
