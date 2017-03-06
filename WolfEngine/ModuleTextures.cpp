#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "OpenGL.h"
#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>

#pragma comment( lib, "DevIL/libx86/DevIL.lib" )
#pragma comment( lib, "DevIL/libx86/ILU.lib" )
#pragma comment( lib, "DevIL/libx86/ILUT.lib" )

ModuleTextures::ModuleTextures() : Module(MODULE_TEXTURES)
{
}

ModuleTextures::~ModuleTextures()
{
}

bool ModuleTextures::Init()
{
	LOG("Init Image library via DevIL library");
	bool ret = true;

	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

	LoadCheckers();
	texture_debug = LoadTexture(aiString("Resources/Lenna.png"));

	return ret;
}

bool ModuleTextures::CleanUp()
{
	LOG("Freeing textures and Image library");

	ilShutDown();

	return true;
}

unsigned int ModuleTextures::LoadTexture(const aiString& path)
{
	unsigned int ret = 0;

	TextureList::iterator it = textures.find(path);

	if (it == textures.end())
	{
		ILuint imageId = ilGenImage();
		ilBindImage(imageId);
		ilLoadImage(path.data);

		ILenum Error = ilGetError();
		if (Error != IL_NO_ERROR)
			LOG("Error %d: %s", Error, iluErrorString(Error));

		ret = ilutGLBindTexImage();

		textures[path] = ret;

		Error = ilGetError();
		if (Error != IL_NO_ERROR)
			LOG("Error %d: %s", Error, iluErrorString(Error));

		LOG("Load texture key %s with value %d", path.data, ret);

		ilDeleteImage(imageId);
	}
	else
	{
		ret = textures[path];
		LOG("Texture key %s already loaded with value %d", path.data, ret);
	}

	return ret;
}

void ModuleTextures::LoadCheckers()
{
	const int CHECKERS_HEIGHT = 16;
	const int CHECKERS_WIDTH = 16;

	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texture_checkers);
	glBindTexture(GL_TEXTURE_2D, texture_checkers);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glBindTexture(GL_TEXTURE_2D, 0);
}

