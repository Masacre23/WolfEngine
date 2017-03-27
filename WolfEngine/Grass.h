#ifndef GRASS_H
#define GRASS_H

#include <list>
#include "Math.h"

class Billboard;

class Grass
{
public:
	struct CompareDepth {
		bool operator()(Billboard* b, Billboard* b2) { return b->position.normalize() < b2->position.normalize(); }
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