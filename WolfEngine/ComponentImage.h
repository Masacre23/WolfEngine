#ifndef COMPONENTIMAGE_H
#define COMPONENTIMAGE_H
#include "Component.h"
#include "OpenGL.h"

class ComponentRectTransform;

class ComponentImage : public Component
{
public:
	ComponentImage(GameObject* parent);
	~ComponentImage();

	void OnDraw() const;
	bool OnEditor();

public:
	char* path = "Resources/Default.png";
	GLuint texture;
	ComponentRectTransform* rect_transform = nullptr;
};

#endif