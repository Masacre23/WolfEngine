#include "ComponentParticleSystem.h"
#include "Billboard.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "OpenGL.h"
#include <stdlib.h>

ComponentParticleSystem::ComponentParticleSystem(GameObject * parent) : Component(Component::Type::PARTICLE, parent)
{
}

ComponentParticleSystem::~ComponentParticleSystem()
{
}

void ComponentParticleSystem::Init(unsigned max_particles, const aiVector2D & _emit_size, unsigned _falling_time, float falling_height, const char * texture_file, const aiVector2D & psize)
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
		b->position.x = rand() % 10 - 5;
		b->position.y = rand() % 2 + falling_height;
		b->position.z = rand() % 10 - 5;

		b->position += App->camera->GetPosition();
	}
	b->position.y -= 0.1f;
}

bool ComponentParticleSystem::OnDraw()
{
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.8f);
	for (int i = 0; i < particles.size(); ++i)
	{
		Rain(particles[i].billboard);
		particles[i].billboard->ComputeQuad(App->camera->GetPosition());
		particles[i].billboard->Draw();
	}
	glDisable(GL_ALPHA_TEST);
	return false;
}

bool ComponentParticleSystem::OnEditor()
{
	if (ImGui::CollapsingHeader("Particle System"))
	{
		ImGui::Checkbox("Active", &enable);

		ImGui::SameLine();

		if (ImGui::Button("Delete"))
			this->~ComponentParticleSystem();

		static int maxparticles = 0;

		ImGui::SliderInt("Max particles", &maxparticles, 1, 1000);
		
		static int x = emit_area.x, z = emit_area.y;
		ImGui::SliderInt("Emit X", &x, -100, 100);
		ImGui::SliderInt("Emit Z", &z, -100, 100);
		emit_area = aiVector2D(x, z);

		//ImGui::InputInt("Fallint time", &falling_time);


		if (ImGui::Button("Save"))
			Init(maxparticles, emit_area, falling_time, falling_height, "Resources/rainSprite.tga", aiVector2D(0.5f, 0.5f));
		//ImGui::SliderInt("Lines", &lines, 1, 10);
		//ImGui::SameLine();
		//ImGui::SliderInt("Cols", &cols, 1, 10);

		//if (ImGui::Button("Update"))
		//	this->Enable();
	}

	return ImGui::IsItemClicked();
}
