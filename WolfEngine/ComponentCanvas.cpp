#include "ComponentCanvas.h"

ComponentCanvas::ComponentCanvas(GameObject * parent) : Component(Component::Type::CANVAS, parent)
{
}

ComponentCanvas::~ComponentCanvas()
{
}

void ComponentCanvas::OnDraw() const
{
}

bool ComponentCanvas::OnEditor()
{
	return false;
}
