#ifndef MODULEPROGRAMSHADERS_H
#define MODULEPROGRAMSHADERS_H

#define MODULE_PROGRAM_SHADERS "ModuleProgramShaders"

#include "Module.h"
#include <assimp/types.h>
#include <map>

class ModuleProgramShaders : public Module
{

	struct LessString
	{
		bool operator()(const aiString& left, const aiString& right) const
		{
			return ::strcmp(left.data, right.data) < 0;
		}
	};

public:
	ModuleProgramShaders();
	~ModuleProgramShaders();

	bool Init();
	bool CleanUp();

	void Load(const char* name, const char* vertex_shader, const char* fragment_shader);
	
	int GetUniformLocation(const char* name, const char* vertex_shader, const char* fragment_shader);
	void UseProgram(const char* name);
	void UnuseProgram();

private:
	typedef std::map<aiString, unsigned int, LessString> ProgramList;
	ProgramList programs;

};

#endif // !MODULEPROGRAMSHADERS_H

