#include "MyQuadTree.h"
#include "Application.h"
#include "ModuleRender.h"
#include "GameObject.h"



MyQuadTree::MyQuadTree(AABB bbox)
{
	root = new QuadNode();
	root->limit = bbox;
}

MyQuadTree::~MyQuadTree()
{
	RELEASE(root);
}

void MyQuadTree::Draw()
{
	std::vector<QuadNode*> quadnodes;
	quadnodes.push_back(root);
	QuadNode* node = nullptr;
	while (!quadnodes.empty()) 
	{
		node = quadnodes.back();
		quadnodes.pop_back();
		if (node->children != nullptr)
		{
			for (int i = 0; i < 4; ++i)
				quadnodes.push_back(node->children[i]);
		}
		App->renderer->DrawBoundingBox(node->limit);
	}
}

void MyQuadTree::Insert(GameObject * game_object)
{
	std::vector<QuadNode*> quadnodes;
	std::vector<GameObject*> game_objects;
	quadnodes.push_back(root);
	QuadNode* node = nullptr;
	GameObject* go = game_object;
	while (go != nullptr && !quadnodes.empty()) {
		node = quadnodes.back();
		if (node->limit.Contains(go->bbox))
		{
			if (node->children != nullptr)
			{
				quadnodes.pop_back();
				for (int i = 0; i < 4; ++i)
					quadnodes.push_back(node->children[i]);
			}
			else
			{
				if (node->bucket_size == BUCKET_SPACE)
				{
					quadnodes.pop_back();
					node->children = new QuadNode*[4];
					float z_4 = (node->limit.MaxZ() - node->limit.MinZ()) / 4;
					float z_3_4 = z_4 * 3;
					float x_4 = (node->limit.MaxX() - node->limit.MinX()) / 4;
					float x_3_4 = x_4 * 3;
					float3 size = node->limit.HalfSize();
					size.y = 2 * size.y;
					for (int i = 0; i < 4; ++i)
					{
						node->children[i] = new QuadNode();
						float3 center = float3(i%2 == 0 ? x_4:x_3_4, node->limit.CenterPoint().y, i<2 ? z_4:z_3_4);
						node->children[i]->limit.SetFromCenterAndSize(center, size);
						quadnodes.push_back(node->children[i]);
						node->children[i]->bucket = new GameObject*[BUCKET_SPACE];
					}
					for (int i = 0; i < node->bucket_size; ++i)
						game_objects.push_back(node->bucket[i]);
				}
				else
				{
					node->bucket[node->bucket_size] = go;
					node->bucket_size++;
					if (game_objects.empty())
						go = nullptr;
					else
					{
						go = game_objects.back();
						game_objects.pop_back();
					}
				}
			}
		}
		else 
		{
			quadnodes.pop_back();
		}
	}
	quadnodes.clear();
}

void MyQuadTree::IntersectCandidates(std::vector<GameObject*>& candidates, AABB primitive)
{
}
