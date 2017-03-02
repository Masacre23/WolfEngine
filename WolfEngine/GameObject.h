#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <string>
#include "Component.h"

class Component;

class GameObject
{
public:
	GameObject();
	GameObject(const std::string& name);
	~GameObject();

	bool Update();
	Component* CreateComponent(TypeComponent type);

private:
	std::string name = "GameObject";
	GameObject* parent;
	bool active;

	std::vector<Component*> components;
	std::vector<GameObject*> childs;
};



#endif // !GAMEOBJECT_H
