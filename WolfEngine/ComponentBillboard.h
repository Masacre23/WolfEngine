#ifndef COMPONENTBILLBOARD_H
#define COMPONENTBILLBOARD_H

#include <list>
#include "Math.h"
#include "Billboard.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "Component.h"

class GameObject;

class ComponentBillboard : public Component
{
public:
	struct CompareDepth {
		bool operator()(Billboard* b, Billboard* b2) { return (b->position - App->camera->GetPosition()).Length() > (b2->position - App->camera->GetPosition()).Length(); }
	};

	ComponentBillboard(GameObject* parent, int lines, int cols);
	~ComponentBillboard();

	void Enable();
	bool OnDraw()const;
	bool OnEditor();

public:
	int lines;
	int cols;
	int num_billboards;

	Billboard* billboard;
	std::list<std::list<Billboard*>> grid;
};

#endif