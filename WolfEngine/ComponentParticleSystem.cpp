#include "ComponentParticleSystem.h"
#include "Billboard.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "Interface.h"
#include "OpenGL.h"
#include <stdlib.h>

ComponentParticleSystem::ComponentParticleSystem(GameObject * parent) : Component(Component::Type::PARTICLE, parent)
{
}

ComponentParticleSystem::~ComponentParticleSystem()
{
}

void ComponentParticleSystem::Init(unsigned max_particles, const float2 & _emit_size, unsigned _falling_time, float falling_height, const char * texture_file, const float2 & psize)
{
	this->emit_area = _emit_size;
	this->falling_time = _falling_time;
	this->falling_height = falling_height;
	particles.clear();

	for (int i = 0; i < max_particles; ++i)
	{
		Particle p = Particle();
		float3 localPos = float3(rand() % 10 - 5, rand() % 10, rand() % 10 - 5);
		p.position = App->camera->GetPosition() + localPos;
		p.billboard = new Billboard((aiString)texture_file, p.position, 1, 1);
		p.billboard->time_reset_pos = rand() % 100;
		p.lifetime = 0.1f;
		p.velocity = float3(0,0,0);
		particles.push_back(p);
	}
}

void ComponentParticleSystem::Clear()
{
}

void ComponentParticleSystem::Rain(Billboard* b)
{
	if (b->position.y <= 0)
	{
		b->position.x = (rand() % 10 - 5) / scale.x;
		b->position.y = (rand() % 2 + falling_height) / scale.y;
		b->position.z = (rand() % 10 - 5) / scale.z;

		float3 aux = { App->camera->GetPosition().x / scale.x, App->camera->GetPosition().y / scale.y, App->camera->GetPosition().z / scale.z};
		b->position += aux;
	}
	b->position.y -= 0.1f / scale.y;
}

void ComponentParticleSystem::OnDraw()
{
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	for (int i = 0; i < particles.size(); ++i)
	{
		Rain(particles[i].billboard);
		particles[i].billboard->ComputeQuad(App->camera->GetPosition());
		particles[i].billboard->Draw(scale, texture_scale);
	}
	glDisable(GL_ALPHA_TEST);
}

bool ComponentParticleSystem::OnEditor()
{
	if (ImGui::CollapsingHeader("Particle System"))
	{
		ImGui::Checkbox("Active##Particles", &enable);

		ImGui::SameLine();

		if (ImGui::Button("Delete##Particles"))
			this->~ComponentParticleSystem();

		ImGui::SliderInt("Max particles", (int*)&maxparticles, 1, 1000);
		ImGui::DragFloat2("Emit area", (float*)&emit_area.x, 1, -100, 100);
		ImGui::DragInt("Falling time", (int*)&falling_time, 1.0f);
		/*char buf[1024];
		strcpy(buf, (const char*)texture_file);
		ImGui::InputText("", buf, IM_ARRAYSIZE(buf));
		texture_file = buf;*/
		ImGui::DragFloat3("Scale", (float*)&scale, 0.1f, 0.1f, 1.0f);
		ImGui::DragFloat2("Texture", (float*)&texture_scale, 0.1f, 0.0f, 100.0f);
		//ImGui::InputInt("Fallint time", &falling_time);

		if (ImGui::Button("Save"))
			Init(maxparticles, emit_area, falling_time, falling_height, texture_file, texture_scale);
	}

	return ImGui::IsItemClicked();
}
