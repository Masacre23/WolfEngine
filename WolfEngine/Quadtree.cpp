#include "Application.h"
#include "ModuleRender.h"
#include "Quadtree.h"
#include "GameObject.h"
#include "Math.h"

Quadtree::Quadtree(const AABB& quadtree_limits)
{
	root_node = new QuadtreeNode(quadtree_limits);
}

Quadtree::~Quadtree()
{
	RELEASE(root_node);
}

void Quadtree::Draw()
{
	root_node->Draw();
}

void Quadtree::Insert(GameObject* game_object)
{
	root_node->Insert(game_object);
}

QuadtreeNode::QuadtreeNode(const AABB& node_limits) : node_box(node_limits)
{
}

QuadtreeNode::~QuadtreeNode()
{
	for (std::vector<QuadtreeNode*>::iterator it = childs.begin(); it != childs.end(); ++it)
		RELEASE(*it);
	childs.clear();
}

void QuadtreeNode::Draw()
{
	App->renderer->DrawBoundingBox(node_box);

	for (std::vector<QuadtreeNode*>::iterator it = childs.begin(); it != childs.end(); ++it)
		(*it)->Draw();
}

void QuadtreeNode::Insert(GameObject* game_object)
{
	if (node_box.Contains(game_object->bbox))
	{
		if (childs.size() == 0)
		{
			if (objects.size() + 1 > BUCKETSIZE)
			{
				CreateChilds();
				for (int i = 0; i < 4; i++)
					childs[i]->Insert(game_object);
			}
			else
				objects.push_back(game_object);
		}
		else
		{
			for (int i = 0; i < 4; i++)
				childs[i]->Insert(game_object);
		}
	}
}

void QuadtreeNode::CreateChilds()
{
	float3 center = node_box.CenterPoint();
	float3 min_point = float3(center.x, 0, center.z);

	float3 corners[8];
	node_box.GetCornerPoints(corners);

	childs.push_back(new QuadtreeNode(AABB(min_point, corners[2])));
	childs.push_back(new QuadtreeNode(AABB(min_point, corners[3])));
	childs.push_back(new QuadtreeNode(AABB(min_point, corners[6])));
	childs.push_back(new QuadtreeNode(AABB(min_point, corners[7])));
		
}
