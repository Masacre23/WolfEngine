#ifndef COMPONENTIMAGE_H
#define COMPONENTIMAGE_H
#include "Component.h"

class ComponentImage : public Component
{
public:
	ComponentImage(GameObject* parent);
	~ComponentImage();
};

#endif