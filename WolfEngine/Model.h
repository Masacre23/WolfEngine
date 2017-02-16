#ifndef MODEL_H
#define MODEL_H

#include <vector>

class aiScene;

class Model
{
public:
	Model();
	~Model();

	void Load(const char* file);
	void Clear();
	void Draw();

private:
	const aiScene* scene = nullptr;
	std::vector<std::vector<unsigned int>> indices;

};

#endif // !MODEL_H

