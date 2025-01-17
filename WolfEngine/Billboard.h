#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "Math.h"
#include "assimp/types.h"

class Billboard
{

public:
	Billboard(aiString& texture, float3 position, float up, float right);
	~Billboard();

	void ComputeQuad(float3 camera);
	void Draw(float3 scale, float2 texture_scale);

public:
	float3 position;
	unsigned time_reset_pos = 0;
private:
	const float3 up_vector = float3(0.0f, 1.0f, 0.0f);
	unsigned int texture;
	float3 vertices[4];
	float up, right;
};

#endif // !BILLBOARD_H

