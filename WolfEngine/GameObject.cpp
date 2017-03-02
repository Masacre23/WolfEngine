#include "GameObject.h"
#include "ComponentTransform.h"

GameObject::GameObject()
{
}

GameObject::GameObject(const std::string& name) : name(name)
{
}

GameObject::~GameObject()
{
}

bool GameObject::Update()
{
	return false;
}

Component* GameObject::CreateComponent(TypeComponent type)
{
	Component* ret = new Component(type);

	components.push_back(ret);

	return ret;
}
