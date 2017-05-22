#include "ComponentImage.h"
#include "Interface.h"

ComponentImage::ComponentImage(GameObject* parent) : Component(Component::Type::IMAGE, parent)
{
}

ComponentImage::~ComponentImage()
{
}

/*bool ComponentImage::OnEditor()
{
	bool node_open = ImGui::CollapsingHeader("Rect Transform");

	if (node_open)
	{
	}

	return ImGui::IsItemClicked();
}*/