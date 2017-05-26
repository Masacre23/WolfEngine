#ifndef COMPONENTIMAGE_H
#define COMPONENTIMAGE_H
#include "Component.h"
#include "OpenGL.h"

class ComponentImage : public Component
{
public:
	ComponentImage(GameObject* parent);
	~ComponentImage();

	void OnDraw() const;
	bool OnEditor();

public:
	char* path = "Resources/Lenna.png";
	GLuint texture;
};

#endif