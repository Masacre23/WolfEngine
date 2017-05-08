#ifndef COMPONENTPARTICLESYSTEM_H
#define COMPONENTPARTICLESYSTEM_H

#include "Component.h"
//#include "assimp\types.h"
#include <list>
#include <vector>
#include "Math.h"

class ComponentCamera;
class Billboard;

struct Particle
{
	Billboard* billboard;
	float3 position;
	float3 velocity;
	unsigned lifetime;
};

class ComponentParticleSystem : public Component
{
public:
	/*struct CompareDepth {
		bool operator()(Particle* b, Particle* b2) { return (b->position - App->camera->GetPosition()).Length() > (b2->position - App->camera->GetPosition()).Length(); }
	};*/

	ComponentParticleSystem(GameObject* parent);
	~ComponentParticleSystem();

	void Init(unsigned max_particles, const float2& _emit_size, unsigned _falling_time, float falling_height, const char* texture_file, const float2& psize);
	void Clear();
	void Rain(Billboard* b);
	bool OnDraw();
	bool OnEditor();

public:
	typedef std::vector<Billboard> BillboardList;
	typedef std::vector<Particle> ParticlePool;
	typedef std::vector<unsigned> ParticleList;

	ParticlePool particles;
	ParticleList alive;
	ParticleList dead;
	BillboardList billboards;

	unsigned maxparticles = 1;
	float2 emit_area = {1.0f, 1.0f};
	unsigned falling_time = 0;
	unsigned accum_elapsed = 0;
	float falling_height = 0.0f;
	char * texture_file = "Resources/rainSprite.tga";
	float3 scale = {1.0f, 1.0f, 1.0f};
	float2 texture_scale = {6.0f, 2.0f};

	float3* vertices = nullptr;
	float2* text_coords = nullptr;
	float4* colors = nullptr;
	unsigned* indices = nullptr;
};

#endif