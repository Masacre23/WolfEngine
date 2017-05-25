#include "ComponentText.h"
#include "Interface.h"
#include "freetype.h"
#include "GameObject.h"
#include "ComponentRectTransform.h"

freetype::font_data font;

ComponentText::ComponentText(GameObject* parent) : Component(Component::Type::TEXT, parent)
{
	font.init("Test.ttf", 16);
}

ComponentText::~ComponentText()
{
}

void ComponentText::OnDraw() const
{
	// Blue text
	glColor3ub(color[0], color[1], color[2]);

	glPushMatrix();
	glLoadIdentity();
	ComponentRectTransform* rt = (ComponentRectTransform*)parent->GetComponent(Component::Type::RECT_TRANSFORM);
	freetype::print(font, rt->pos[0], rt->pos[1], text);
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

		ImGui::InputInt3("Color", color);
	}

	return ImGui::IsItemClicked();
}
