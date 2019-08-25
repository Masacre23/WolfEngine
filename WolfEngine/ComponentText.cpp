#include "ComponentText.h"
#include "Interface.h"
#include "freetype.h"
#include "GameObject.h"
#include "ComponentRectTransform.h"

freetype::font_data font;

ComponentText::ComponentText(GameObject* parent) : Component(Component::Type::TEXT, parent)
{
	font.init("Test.ttf", size);
	rect_transform = (ComponentRectTransform*)parent->GetComponent(Component::Type::RECT_TRANSFORM);
}

ComponentText::~ComponentText()
{
}

void ComponentText::OnDraw() const
{
	glColor3ub(color[0], color[1], color[2]);

	glPushMatrix();
	glLoadIdentity();
	freetype::print(font, rect_transform->pos[0], rect_transform->pos[1], text);
	glPopMatrix();
}

bool ComponentText::OnEditor()
{
	bool node_open = ImGui::CollapsingHeader("Text");

	if (node_open)
	{
		static char buf[64];
		strcpy(buf, (const char*)text);
		ImGui::InputText("", buf, IM_ARRAYSIZE(buf));
		text = buf;

		ImGui::DragInt3("Color", color);

		ImGui::DragInt("Size", &size);

		if(ImGui::Button("Apply"))
			font.init("Test.ttf", size);
	}

	return ImGui::IsItemClicked();
}
