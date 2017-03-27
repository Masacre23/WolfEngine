#include "Grass.h"

Grass::Grass(int lines, int cols) : lines(lines), cols(cols)
{
//	billboard = new Billboard();
}


Grass::~Grass()
{
}

void Grass::Start()
{
	std::list<Billboard*> aux;
	for (int i = 0; i < lines * cols; ++i)
	{
		aux.push_back(billboard);
	}

	aux.sort(CompareDepth());

	for (auto a = aux.begin(); a != aux.end(); ++a)
	{
		std::list<Billboard*> v_line;
		for(int i = 0; i < cols; ++i)
		{
			v_line.push_back(*a);
			++a;
		}
		grid.push_back(v_line);
	}
}

void Grass::Draw()
{
	for (auto l = grid.begin(); l != grid.end(); ++l)
	{
		for (auto c = l->begin(); c != l->end(); ++c)
		{
			c->Draw();
		}
	}
}
