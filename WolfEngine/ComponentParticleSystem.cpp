#include "ComponentParticleSystem.h"
#include "Billboard.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"
#include "OpenGL.h"

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
		p.position = App->camera->GetPosition();
		p.billboard = new Billboard((aiString)texture_file, p.position, 1, 1);
		p.lifetime = 0.1f;
		p.velocity = float3(0,0,0);
		particles.push_back(p);
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
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	for (int i = 0; i < billboards.size(); ++i)
	{
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

		//ImGui::SliderInt("Lines", &lines, 1, 10);
		//ImGui::SameLine();
		//ImGui::SliderInt("Cols", &cols, 1, 10);

		//if (ImGui::Button("Update"))
		//	this->Enable();
	}

	return ImGui::IsItemClicked();
}
