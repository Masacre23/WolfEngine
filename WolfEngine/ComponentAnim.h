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

	bool OnEditor(int selection_mask, int id);

	bool OnAnimationUpdate();

	void SetName(const char* name);
	void Play(bool loop);
	void Stop();
	bool IsPlaying();
	void BlendTo(const char* name, unsigned int duration);

private:
	aiString anim_name;
	int anim_id = -1;
};

#endif




