#ifndef MATERIAL_H
#define MATERIAL_H

#include <assimp/types.h>

class Material
{
public:
	Material() {}
	~Material() {}

public:
	GLfloat ambient[4]{0.0f,0.0f,0.0f,0.0f};
	GLfloat diffuse[4]{ 0.0f,0.0f,0.0f,0.0f };
	GLfloat specular[4]{ 0.0f,0.0f,0.0f,0.0f };
	float shiness = 0.0f;
	bool has_ambient = false;
	bool has_diffuse = false;
	bool has_specular = false;
	bool has_shiness = false;
};

#endif // !MATERIAL_H
