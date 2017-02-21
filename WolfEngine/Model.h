#ifndef MODEL_H
#define MODEL_H

class Material;
class aiScene;

class Model
{
public:
	Model();
	~Model();

	void Load(const char* folder, const char* file);
	void Clear();
	void Draw();

private:
	const aiScene* scene = nullptr;
	unsigned int** indices;
	unsigned int** textures;
	Material* materials;
};

#endif // !MODEL_H

