#ifndef COMPONENTANIM_H
#define COMPONENTANIM_H

#include "Component.h"
#include <list>
#include <vector>
#include <assimp/types.h>
#include "Math.h"

class ComponentAnim : public Component
{
public:
	ComponentAnim(GameObject* parent);
	~ComponentAnim();

	bool OnEditor();
	void SaveComponent();
	void RestoreComponent();

	bool OnAnimationUpdate();

	void LoadAnimations(const char* animation);
	void LoadAnimations(const std::list<std::string>& animations);

	void SetName(const char* name);
	void PlayAnimation(const char* name, bool loop);
	void PlayCurrent(bool loop);
	void StopCurrent();
	bool IsPlaying() const;
	void BlendTo(const char* name, unsigned int duration);

public:
	bool draw_bones = true;

private:
	std::vector<GameObject*> nodes;
	std::list<aiString> animations;
	aiString current_animation;
	int anim_id = -1;
	int blend_time = 200;

	aiString backed_animation;
	int backed_blend_time;
	bool backed_is_playing;
};

#endif




