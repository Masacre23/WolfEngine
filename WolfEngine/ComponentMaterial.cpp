#include "ComponentMaterial.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleProgramShaders.h"
#include "ModuleCamera.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "OpenGL.h"
#include "GameObject.h"
#include "Interface.h"

ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(Component::Type::MATERIAL, parent)
{
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::Load(aiMaterial* material, const aiString& folder_path)
{
	aiColor4D ambient;
	aiColor4D diffuse;
	aiColor4D specular;
	material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
	material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
	if (material->Get(AI_MATKEY_SHININESS, shiness) == AI_SUCCESS)
		shiness *= 128.0f;
	float shine_strength = 1.0f;
	if (material->Get(AI_MATKEY_SHININESS_STRENGTH, shine_strength) == AI_SUCCESS)
		specular *= shine_strength;
	for (int i = 0; i < 3; i++)
	{
		this->ambient[i] = ambient[i];
		this->diffuse[i] = diffuse[i];
		this->specular[i] = specular[i];
	}

	unsigned num_textures = material->GetTextureCount(aiTextureType_DIFFUSE);
	if (num_textures >= 1)
	{
		aiString path;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		{
			aiString full_path = aiString(folder_path);
			full_path.Append(path.data);

			LoadTexture(full_path);
		}
	}
}

void ComponentMaterial::LoadTexture(const aiString& texture_path)
{
	texture = App->textures->LoadTexture(texture_path);
}

void ComponentMaterial::OnDraw() const
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shiness);

	glBindTexture(GL_TEXTURE_2D, texture);

	if (has_shader) {
		App->program_shaders->UseProgram("Prueba");
		glUniform4f(App->program_shaders->GetUniformLocation("Prueba", "light_position"), 1, 1, 1, 0);
		float3 camera = App->camera->GetPosition();
		glUniform3f(App->program_shaders->GetUniformLocation("Prueba", "camera"), camera.x, camera.y, camera.z );
		glUniform1i(App->program_shaders->GetUniformLocation("Prueba", "tex_coord"), 0);
	}
}

bool ComponentMaterial::OnEditor()
{
	if (on_editor)
	{
		if (ImGui::CollapsingHeader("Material"))
		{
			ImGui::Checkbox("Active##Material", &enable);

			ImGui::SameLine();

			if (ImGui::Button("Delete##Material"))
				parent->DeleteComponent(this);

			ImGui::Checkbox("Has Shader", &has_shader);

			ImGui::DragFloat4("Ambient", (float*)&ambient, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat4("Diffuse", (float*)&diffuse, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat4("Specular", (float*)&specular, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Shiness", (float*)&shiness, 1.0f, 0.0f, 128.0f);
		}

		return ImGui::IsItemClicked();
	}
	
	return false;
}

void ComponentMaterial::SaveComponent()
{
	for (int i = 0; i < 4; i++)
	{
		backed_ambient[i] = ambient[i];
		backed_diffuse[i] = diffuse[i];
		backed_specular[i] = specular[i];
	}
	backed_shiness = shiness;
}

void ComponentMaterial::RestoreComponent()
{
	for (int i = 0; i < 4; i++)
	{
		ambient[i] = backed_ambient[i];
		diffuse[i] = backed_diffuse[i];
		specular[i] = backed_specular[i];
	}
	shiness = backed_shiness;

	enable = true;
	on_editor = true;
}
