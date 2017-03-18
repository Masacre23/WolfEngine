#ifndef COMPONENTANIM_H
#define COMPONENTANIM_H

#include "Component.h"
#include <assimp/types.h>
#include "Math.h"

class ComponentAnim : public Component
{
public:
	ComponentAnim(GameObject* parent);
	~ComponentAnim();

	bool OnUpdate();
	bool OnEditor(int selection_mask, int id);

	void SetName(const char* name);
	void Play(bool loop);

private:
	aiString anim_name;
	int anim_id = -1;
};

#endif




