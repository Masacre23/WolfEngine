#ifndef QUADTREE_H
#define QUADTREE_H

#define BUCKETSIZE 1

#include "MathGeoLib\src\Geometry\AABB.h"
#include <list>
#include <vector>

class GameObject;

class QuadtreeNode
{
public:
	QuadtreeNode(const AABB& node_limits);
	~QuadtreeNode();

	void Draw();
	void Insert(GameObject* game_object);

private:
	void CreateChilds();

private:
	AABB node_box;
	std::list<GameObject*> objects;
	std::vector<QuadtreeNode*> childs;
};

class Quadtree
{
public:
	Quadtree(const AABB& quadtree_limits);
	~Quadtree();

	void Draw();
	void Insert(GameObject* game_object);

private:
	QuadtreeNode* root_node;
};

#endif // !QUADTREE_H
