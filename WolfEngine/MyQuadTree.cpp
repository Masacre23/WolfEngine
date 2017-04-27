#include "MyQuadTree.h"
#include "Application.h"
#include "ModuleRender.h"
#include "GameObject.h"
#include "Color.h"
#include <queue>



MyQuadTree::MyQuadTree(AABB bbox)
{
	root = new QuadNode();
	root->limit = bbox;
	root->bucket = new GameObject*[BUCKET_SPACE];
}

MyQuadTree::~MyQuadTree()
{
	RELEASE(root);
}

void MyQuadTree::Draw() const
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
		App->renderer->DrawBoundingBox(node->limit, Colors::Aqua);
	}
}

void MyQuadTree::Insert(GameObject * game_object)
{
	std::queue<QuadNode*> quadnodes;
	std::vector<GameObject*> game_objects;
	QuadNode* node = nullptr;
	QuadNode* last_node = nullptr;
	GameObject* go = game_object;
	while (go != nullptr)
	{
		quadnodes = std::queue<QuadNode*>();
		node = root;
		while (node != nullptr) 
		{
			if (node->limit.Contains(go->bbox))
			{
				last_node = node;
				if (node->children != nullptr) 
				{
					for (int i = 0; i < 4; ++i)
						quadnodes.push(node->children[i]);
				}
				else
				{
					if (node->bucket_size >= BUCKET_SPACE)
					{
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
							float3 center = float3(node->limit.MinX() + (i % 2 == 0 ? x_4 : x_3_4), node->limit.CenterPoint().y, node->limit.MinZ() + (i<2 ? z_4 : z_3_4));
							node->children[i]->limit.SetFromCenterAndSize(center, size);
							quadnodes.push(node->children[i]);
							node->children[i]->bucket = new GameObject*[BUCKET_SPACE];
						}
						for (int i = 0; i < node->bucket_size; ++i)
							game_objects.push_back(node->bucket[i]);
						RELEASE(node->bucket);
						node->bucket = new GameObject*[BUCKET_SPACE];
						node->bucket_size = 0;
					}
				}
			}
			if(!quadnodes.empty())
			{
				node = quadnodes.front();
				quadnodes.pop();
			}
			else 
			{
				node = nullptr;
			}
		}
		
		if (last_node != nullptr)
		{
			if (last_node->bucket_size == last_node->bucket_mul * BUCKET_SPACE)
			{
				last_node->bucket_mul++;
				GameObject** bucket_aux = last_node->bucket;
				last_node->bucket =	new GameObject*[last_node->bucket_mul * BUCKET_SPACE];
				std::memcpy(last_node->bucket, bucket_aux, (last_node->bucket_mul - 1)*BUCKET_SPACE * sizeof(int));
				RELEASE_ARRAY(bucket_aux);
			}
			last_node->bucket[last_node->bucket_size] = go;
			last_node->bucket_size++;
			last_node = nullptr;
		}
		if (game_objects.empty())
			go = nullptr;
		else
		{
			go = game_objects.back();
			game_objects.pop_back();
		}
	}
}

void MyQuadTree::IntersectCandidates(std::vector<GameObject*>& candidates, AABB primitive) const
{
	std::vector<QuadNode*> quadnodes;
	QuadNode* node = root;
	while (node != nullptr)
	{
		if (primitive.Intersects(node->limit))
		{
			for (int i = 0; i < node->bucket_size; i++)
			{
				candidates.push_back(node->bucket[i]);
			}
			if(node->children != nullptr)
				for (int i = 0; i < 4; ++i)
					quadnodes.push_back(node->children[i]);
		}
		if (!quadnodes.empty())
		{
			node = quadnodes.back();
			quadnodes.pop_back();
		}
		else 
		{
			node = nullptr;
		}
	}
}
