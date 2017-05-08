#include "ModuleProgramShaders.h"
#include "OpenGL.h"
#include <string>
#include <cstdio>

ModuleProgramShaders::ModuleProgramShaders() : Module(MODULE_PROGRAM_SHADERS)
{

}


ModuleProgramShaders::~ModuleProgramShaders()
{

}

bool ModuleProgramShaders::Init()
{
	return true;
}

bool ModuleProgramShaders::CleanUp()
{
	return true;
}

void ModuleProgramShaders::Load(const char * name, const char * vertex_shader, const char * fragment_shader)
{
	unsigned int ret = 0;

	aiString path = aiString();
	path.Append(name);

	FILE * vertex_file;
	vertex_file = fopen(vertex_shader, "rb");
	if (vertex_file == nullptr) {
		APPLOG("Error opening file %s: %s\n", vertex_shader, strerror(errno));
		return;
	}
	
	fseek(vertex_file, 0, SEEK_END);
	int vertex_file_size = ftell(vertex_file);
	rewind(vertex_file);
	char* buff_vertex = new char[vertex_file_size + 1];
	fread(buff_vertex, sizeof(char), vertex_file_size, vertex_file);
	buff_vertex[vertex_file_size] = '\0';
	fclose(vertex_file);

	int id_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(id_vertex_shader, 1, &buff_vertex, 0);

	RELEASE_ARRAY(buff_vertex);

	glCompileShader(id_vertex_shader);

	GLint is_compiled = 0;
	glGetShaderiv(id_vertex_shader, GL_COMPILE_STATUS, &is_compiled);
	
	GLint max_length = 0;
	glGetShaderiv(id_vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

	//The maxLength includes the NULL character
	char* info_log = new char[max_length];
	glGetShaderInfoLog(id_vertex_shader, max_length, &max_length, &info_log[0]);
	APPLOG("Compile vertex shader: %s", info_log);

	if (is_compiled == GL_FALSE)
	{
		//We don't need the shader anymore.
		glDeleteShader(id_vertex_shader);

		//Use the infoLog as you see fit.

		//In this simple program, we'll just leave
		return;
	}

	FILE * fragment_file;
	fragment_file = fopen(fragment_shader, "rb");
	if (fragment_file == nullptr) {
		APPLOG("Error opening file %s: %s\n", fragment_shader, strerror(errno));
		return;
	}

	fseek(fragment_file, 0, SEEK_END);
	int fragment_file_size = ftell(fragment_file);
	rewind(fragment_file);
	char* buff_fragment = new char[fragment_file_size + 1];
	fread(buff_fragment,sizeof(char), fragment_file_size, fragment_file);
	buff_fragment[fragment_file_size] = '\0';
	fclose(fragment_file);

	int id_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(id_fragment_shader, 1, &buff_fragment, 0);

	RELEASE_ARRAY(buff_fragment);

	glCompileShader(id_fragment_shader);

	is_compiled = 0;
	glGetShaderiv(id_fragment_shader, GL_COMPILE_STATUS, &is_compiled);
	
	max_length = 0;
	glGetShaderiv(id_fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

	//The maxLength includes the NULL character
	RELEASE_ARRAY(info_log);
	info_log = new char[max_length];
	glGetShaderInfoLog(id_fragment_shader, max_length, &max_length, &info_log[0]);
	APPLOG("Compile fragment shader: %s", info_log);
	
	if (is_compiled == GL_FALSE)
	{
		//We don't need the shader anymore.
		glDeleteShader(id_fragment_shader);

		//Use the infoLog as you see fit.

		//In this simple program, we'll just leave
		return;
	}

	//Vertex and fragment shaders are successfully compiled.
	//Now time to link them together into a program.
	//Get a program object.
	int id_program = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(id_program, id_vertex_shader);
	glAttachShader(id_program, id_fragment_shader);

	//Link our program
	glLinkProgram(id_program);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint is_linked = 0;
	glGetProgramiv(id_program, GL_LINK_STATUS, (int *)&is_linked);
	
	max_length = 0;
	glGetProgramiv(id_program, GL_INFO_LOG_LENGTH, &max_length);

	//The maxLength includes the NULL character
	RELEASE_ARRAY(info_log);
	info_log = new char[max_length];
	glGetProgramInfoLog(id_program, max_length, &max_length, &info_log[0]);
	APPLOG("%s", info_log);

	if (is_linked == GL_FALSE)
	{
		//We don't need the program anymore.
		glDeleteProgram(id_program);
		//Don't leak shaders either.
		glDeleteShader(id_vertex_shader);
		glDeleteShader(id_fragment_shader);

		//Use the infoLog as you see fit.

		//In this simple program, we'll just leave
		return;
	}

	//Always detach shaders after a successful link.
	glDetachShader(id_program, id_vertex_shader);
	glDetachShader(id_program, id_fragment_shader);

	
	programs[path] = id_program;
}

int ModuleProgramShaders::GetUniformLocation(const char * name, const char* uniform)
{
	aiString path = aiString();
	path.Append(name);

	ProgramList::iterator it = programs.find(path);

	if (it != programs.end())
	{
		return glGetUniformLocation(programs[path], uniform);
	}
	else
	{
		return 0;
	}
}

void ModuleProgramShaders::UseProgram(const char * name)
{
	aiString path = aiString();
	path.Append(name);

	ProgramList::iterator it = programs.find(path);

	if (it != programs.end())
	{
		glUseProgram(programs[path]);
	}
}

void ModuleProgramShaders::UnuseProgram()
{
	glUseProgram(0);
}
