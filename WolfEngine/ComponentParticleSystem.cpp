#include "ComponentParticleSystem.h"
#include "Billboard.h"
#include "Application.h"
#include "ModuleCamera.h"

ComponentParticleSystem::ComponentParticleSystem(GameObject * parent) : Component(Component::Type::PARTICLE, parent)
{
}

ComponentParticleSystem::~ComponentParticleSystem()
{
}

void ComponentParticleSystem::Init(unsigned max_particles, const aiVector2D & _emit_size, unsigned _falling_time, float falling_height, const char * texture_file, const aiVector2D & psize)
{
	for (int i = 0; i < max_particles; ++i)
	{
		Particle p = Particle();
		p.position.x = App->camera->GetPosition().x;
		p.position.y = App->camera->GetPosition().y;
		p.position.z = App->camera->GetPosition().z;
		particles.push_back(p);
		billboards.push_back(Billboard((aiString)texture_file, p.position, 1, 1));
	}
}

void ComponentParticleSystem::Clear()
{
}

void ComponentParticleSystem::Update(unsigned elapsed, const ComponentCamera & camera)
{

}

bool ComponentParticleSystem::OnDraw()
{
	for (int i = 0; i < billboards.size(); ++i)
	{
		billboards[i].ComputeQuad(App->camera->GetPosition());
		billboards[i].Draw();
	}
	return false;
}

bool ComponentParticleSystem::OnEditor()
{
	return false;
}
