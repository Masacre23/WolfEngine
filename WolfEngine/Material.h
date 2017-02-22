#ifndef MATERIAL_H
#define MATERIAL_H

#include <assimp/types.h>

class Material
{
public:
	Material() {}
	~Material() {}

	void Load(aiMaterial* material)
	{
		aiColor3D ambient3d;
		if (material->Get(AI_MATKEY_COLOR_AMBIENT, ambient3d) == AI_SUCCESS)
		{
			for (int i = 0; i < 3; i++)
				ambient[i] = ambient3d[i];
			ambient[3] = 1.0f;
		}

		aiColor3D diffuse3d;
		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse3d) == AI_SUCCESS)
		{
			for (int i = 0; i < 3; i++)
				diffuse[i] = diffuse3d[i];
			diffuse[3] = 1.0f;
		}

		aiColor3D specular3d;
		if (material->Get(AI_MATKEY_COLOR_SPECULAR, specular3d) == AI_SUCCESS)
		{
			for (int i = 0; i < 3; i++)
				specular[i] = specular3d[i];
			specular[3] = 1.0f;
		}

		if (material->Get(AI_MATKEY_SHININESS, shiness) == AI_SUCCESS)
			shiness *= 128.0f;

		float shine_strength = 1.0f;
		if (material->Get(AI_MATKEY_SHININESS_STRENGTH, shine_strength) == AI_SUCCESS)
		{
			for (int i = 0; i < 4; i++)
				specular[i] *= shine_strength;
		}
	}

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
