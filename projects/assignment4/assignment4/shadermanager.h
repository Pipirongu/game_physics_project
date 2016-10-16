#pragma once

//For reading shader files
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm> //max

#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <vector>
#include <map>

/**
@class ShaderManager

Holds the compiled and linked vertex- and fragment shader
*/
class ShaderManager
{
public:
	/// Destructor
	~ShaderManager();
	
	///Static function to get the static instance
	static ShaderManager* Instance();

	void AddShaderProgram(const char* vertex_shader_program, const char* fragment_shader_path, std::string name_of_shader);
	void ChangeShader(std::string name_of_shader);

	//Handles for shader uniforms
	GLuint ambient_material_loc;
	GLuint specular_material_loc;
	GLuint shininess_loc;

	GLuint mvp_loc;
	GLuint model_loc;
	GLuint view_loc;
	GLuint modelview_loc;
	GLuint picking_color_loc;
	GLuint picking_id_loc;

	GLuint light_position_loc;
	GLuint light_color_loc;
	GLuint light_intensity_loc;
	GLuint texture_sampler_loc;

	GLuint wireframe_color_loc;
	GLuint projection_loc;
	GLuint text_color_loc;

private:
	//map with shaders
	std::map<std::string, GLuint> shaders;

	/// Constructor
	ShaderManager();
	///Private copy constructor to prevent creating an instance other than calling the Instance() function
	ShaderManager(const ShaderManager&);
	///Private assignment constructor to prevent creating an instance other than calling the Instance() function
	ShaderManager& operator=(const ShaderManager&);
};