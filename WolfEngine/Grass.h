#ifndef GRASS_H
#define GRASS_H

#include <list>
#include "Math.h"
#include "Billboard.h"
#include "Application.h"
#include "ModuleCamera.h"

class Grass
{
public:
	struct CompareDepth {
		bool operator()(Billboard* b, Billboard* b2) { return (b->position - App->camera->GetPosition()).Length() < (b2->position - App->camera->GetPosition()).Length(); }
	};

	Grass(int lines, int cols);
	~Grass();

	void Start();
	void Draw();

	//void UpdateRotation();

public:
	int lines;
	int cols;
	int num_billboards;

	Billboard* billboard;
	std::list<std::list<Billboard*>> grid;
	/*std::vector<int> vertices;
	std::vector<int> indices;
	std::vector<float3> tex_coords;*/
};

#endif