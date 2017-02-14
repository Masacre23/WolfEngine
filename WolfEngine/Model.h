#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>

class Model
{
	const aiScene* scene = nullptr;

public:
	Model();
	~Model();

	void Load(const char* file);
	void Clear();
	void Draw();

};

#endif // !MODEL_H

