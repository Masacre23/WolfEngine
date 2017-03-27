#include "Grass.h"
#include "OpenGL.h"


Grass::Grass(int lines, int cols) : lines(lines), cols(cols)
{
	
}


Grass::~Grass()
{
}

void Grass::Start()
{
	std::list<Billboard*> aux;

	for (int i = 0; i < lines * cols; ++i)
	{
		billboard = new Billboard(aiString("Resources/billboardgrass.png"), float3(rand()%10 - 5, 1, rand() % 10 - 5), 1, 1);
		aux.push_back(billboard);
	}

	aux.sort(CompareDepth());

	for (auto a = aux.begin(); a != aux.end(); )
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
	//grid.back().back()->ComputeQuad(App->camera->GetPosition());
//	grid.back().back()->Draw();

	glDisable(GL_DEPTH_TEST);
	for (auto l = grid.begin(); l != grid.end(); ++l)
	{
		for (auto c = l->begin(); c != l->end(); ++c)
		{
			Billboard* b = (Billboard*)(*c);
			b->ComputeQuad(App->camera->GetPosition());
			b->Draw();
		}
	}
	glEnable(GL_DEPTH_TEST);
}
