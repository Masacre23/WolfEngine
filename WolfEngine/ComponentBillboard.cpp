#include "ComponentBillboard.h"
#include "OpenGL.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "Interface.h"

ComponentBillboard::ComponentBillboard(GameObject* parent, int lines, int cols) : lines(lines), cols(cols), Component(Component::Type::BILLBOARD, parent)
{
}


ComponentBillboard::~ComponentBillboard()
{
}

void ComponentBillboard::Enable()
{
	enable = true;
	std::list<Billboard*> aux;

	for (int i = 0; i < lines * cols; ++i)
	{
		billboard = new Billboard(aiString("Resources/billboardgrass.png"), float3(rand()%10 - 5, 1, rand() % 10 - 5) + parent->transform->GetPosition(), 1, 1);
		aux.push_back(billboard);
	}

	aux.sort(CompareDepth());

	for (auto a = aux.begin(); a != aux.end(); )
	{
		std::list<Billboard*> v_line;
		for(int i = 0; i < cols; ++i)
		{
			v_line.push_back(*a);
			++a;
		}
		grid.push_back(v_line);
	}
}

bool ComponentBillboard::OnDraw() const
{
	//grid.back().back()->ComputeQuad(App->camera->GetPosition());
//	grid.back().back()->Draw();

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	for (auto l = grid.begin(); l != grid.end(); ++l)
	{
		for (auto c = l->begin(); c != l->end(); ++c)
		{
			Billboard* b = (Billboard*)(*c);
			b->ComputeQuad(App->camera->GetPosition());
			b->Draw({ 1.0f, 1.0f, 1.0f }, {1.0f, 1.0f});
		}
	}
	glDisable(GL_ALPHA_TEST);

	return true;
}

bool ComponentBillboard::OnEditor()
{
	if (ImGui::CollapsingHeader("Billboard"))
	{
		ImGui::Checkbox("Active", &enable);

		ImGui::SameLine();

		if (ImGui::Button("Delete"))
			this->~ComponentBillboard();

		ImGui::SliderInt("Lines", &lines, 1, 10);
		//ImGui::SameLine();
		ImGui::SliderInt("Cols", &cols, 1, 10);

		if (ImGui::Button("Update"))
			this->Enable();
	}

	return ImGui::IsItemClicked();
}
