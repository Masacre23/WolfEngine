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
	freetype::print(font, 500, 400, text);
	glPopMatrix();
}

bool ComponentText::OnEditor()
{
	bool node_open = ImGui::CollapsingHeader("Text");

	if (node_open)
	{
		static char buf[1024];
		strcpy(buf, (const char*)text);
		ImGui::InputText("", buf, IM_ARRAYSIZE(buf));
		text = buf;
	}

	return ImGui::IsItemClicked();
}
