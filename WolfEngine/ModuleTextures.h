#ifndef MODULETEXTURES_H
#define MODULETEXTURES_H

#include "Module.h"
#include <assimp/types.h>
#include <map>

#define MODULE_TEXTURES "ModuleTextures"

class ModuleTextures : public Module
{
	struct LessString
	{
		bool operator()(const aiString& left, const aiString& right) const
		{
			return ::strcmp(left.data, right.data) < 0;
		}
	};

public:
	ModuleTextures();
	~ModuleTextures();
	
	bool Init();
	bool CleanUp();

	unsigned int LoadTexture(const aiString& path);

private:
	void LoadCheckers();

public:
	unsigned int texture_checkers;
	unsigned int texture_debug;

private:
	typedef std::map<aiString, unsigned int, LessString> TextureList;
	TextureList textures;
};


#endif // !MODULETEXTURES_H