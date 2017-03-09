#include "ComponentMaterial.h"
#include "Application.h"
#include "ModuleTextures.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "OpenGL.h"
#include "GameObject.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl_gl3.h"

ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(Component::Type::MATERIAL, parent)
{
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::Load(aiMaterial * material, const aiString& folder_path)
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

			texture = App->textures->LoadTexture(full_path);
		}
	}
}

bool ComponentMaterial::OnUpdate()
{
	return true;
}

bool ComponentMaterial::OnDraw() const
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shiness);

	glBindTexture(GL_TEXTURE_2D, texture);

	return true;
}

bool ComponentMaterial::OnEditor(int selection_mask, int id)
{
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << id)) ? ImGuiTreeNodeFlags_Selected : 0);
	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)id, node_flags, "Material");

	if (node_open)
	{
		ImGui::Checkbox("Active", &enable);
		ImGui::TreePop();
	}

	return ImGui::IsItemClicked();
}
