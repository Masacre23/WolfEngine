#include "ComponentText.h"
#include "Interface.h"
#include "freetype.h"

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
	glColor3ub(0, 0, 0xff);

	glPushMatrix();
	glLoadIdentity();
	freetype::print(font, 500, 400, "Holiii");
	glPopMatrix();
}

bool ComponentText::OnEditor()
{
	bool node_open = ImGui::CollapsingHeader("Text");

	if (node_open)
	{
	}

	return ImGui::IsItemClicked();
}
