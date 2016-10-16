#include "shadermanager.h"

//------------------------------------------------------------------------------
/**
*/
ShaderManager::ShaderManager()
{
}

//------------------------------------------------------------------------------
/**
*/
ShaderManager::~ShaderManager()
{
}

//------------------------------------------------------------------------------
/**
*/
ShaderManager* ShaderManager::Instance()
{
	static ShaderManager instance;

	return &instance;
}

void ShaderManager::ChangeShader(std::string name_of_shader)
{
	if (this->shaders.find(name_of_shader) == this->shaders.end()){
		printf("Couldn't Find Shader");
		return;
	}
	GLuint shader_id = this->shaders[name_of_shader];
	glUseProgram(shader_id);

	//Get handles for shader uniforms
	this->ambient_material_loc = glGetUniformLocation(shader_id, "ambient_material");
	this->specular_material_loc = glGetUniformLocation(shader_id, "specular_material");
	this->shininess_loc = glGetUniformLocation(shader_id, "shininess");

	this->mvp_loc = glGetUniformLocation(shader_id, "mvp");
	this->model_loc = glGetUniformLocation(shader_id, "model");
	this->view_loc = glGetUniformLocation(shader_id, "view");
	this->modelview_loc = glGetUniformLocation(shader_id, "modelview");
	this->picking_color_loc = glGetUniformLocation(shader_id, "picking_color");
	this->picking_id_loc = glGetUniformLocation(shader_id, "picking_id");

	this->light_position_loc = glGetUniformLocation(shader_id, "light_position_worldspace");
	this->light_color_loc = glGetUniformLocation(shader_id, "light_color");
	this->light_intensity_loc = glGetUniformLocation(shader_id, "light_intensity");

	this->texture_sampler_loc = glGetUniformLocation(shader_id, "texture_sampler");
	this->wireframe_color_loc = glGetUniformLocation(shader_id, "wireframe_color");

	this->projection_loc = glGetUniformLocation(shader_id, "projection");
	this->text_color_loc = glGetUniformLocation(shader_id, "text_color");
}

//------------------------------------------------------------------------------
/**
*/
void ShaderManager::AddShaderProgram(const char* vertex_shader_path, const char* fragment_shader_path, std::string name_of_shader)
{
	// Create the shaders
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string vertex_shader_code;
	std::ifstream vertex_shader_stream(vertex_shader_path, std::ios::in);
	if (vertex_shader_stream.is_open())
	{
		std::string Line = "";
		while (getline(vertex_shader_stream, Line))
			vertex_shader_code += "\n" + Line;
		vertex_shader_stream.close();
	}

	// Read the Fragment Shader code from the file
	std::string fragment_shader_code;
	std::ifstream fragment_shader_stream(fragment_shader_path, std::ios::in);
	if (fragment_shader_stream.is_open()){
		std::string Line = "";
		while (getline(fragment_shader_stream, Line))
			fragment_shader_code += "\n" + Line;
		fragment_shader_stream.close();
	}

	GLint result = GL_FALSE;
	int info_log_length;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_shader_path);
	char const * vertex_source_pointer = vertex_shader_code.c_str();
	glShaderSource(vertex_shader_id, 1, &vertex_source_pointer, NULL);
	glCompileShader(vertex_shader_id);

	// Check Vertex Shader
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE){
		glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> vertex_shader_error_message(info_log_length);
		glGetShaderInfoLog(vertex_shader_id, info_log_length, NULL, &vertex_shader_error_message[0]);
		fprintf(stdout, "%s\n", &vertex_shader_error_message[0]);
		return;
	}
	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_shader_path);
	char const * fragment_source_pointer = fragment_shader_code.c_str();
	glShaderSource(fragment_shader_id, 1, &fragment_source_pointer, NULL);
	glCompileShader(fragment_shader_id);

	// Check Fragment Shader
	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE){
		glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> fragment_shader_error_message(info_log_length);
		glGetShaderInfoLog(fragment_shader_id, info_log_length, NULL, &fragment_shader_error_message[0]);
		fprintf(stdout, "%s\n", &fragment_shader_error_message[0]);
		return;
	}

	// Link the program
	fprintf(stdout, "Linking program\n");
	GLuint program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);
	glLinkProgram(program_id);

	// Check the program
	glGetProgramiv(program_id, GL_LINK_STATUS, &result);
	if (result == GL_FALSE){
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_error_message(std::max(info_log_length, int(1)));
		glGetProgramInfoLog(program_id, info_log_length, NULL, &program_error_message[0]);

		//The program is useless now. So delete it.
		glDeleteProgram(program_id);
		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);

		fprintf(stdout, "%s\n", &program_error_message[0]);
		return;
	}
	//Always detach shaders after a successful link.
	glDetachShader(program_id, vertex_shader_id);
	glDetachShader(program_id, fragment_shader_id);
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	this->shaders[name_of_shader] = program_id;
}