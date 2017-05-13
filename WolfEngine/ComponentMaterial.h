#ifndef COMPONENTMATERIAL_H
#define COMPONENTMATERIAL_H

#include "Component.h"

struct aiMaterial;
struct aiString;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* parent);
	~ComponentMaterial();

	void Load(aiMaterial* material, const aiString& folder_path);
	void LoadTexture(const aiString& texture_path);

	bool OnUpdate();
	bool OnDraw() const;
	bool OnEditor();



private:
	float ambient[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float shiness = 0.0f;
	unsigned texture = 0;
};



#endif // !COMPONENTMATERIAL_H