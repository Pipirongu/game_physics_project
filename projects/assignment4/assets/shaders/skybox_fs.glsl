#version 330

// Interpolated values from the vertex shaders
in vec3 tex_direction;

// Ouput data
layout(location = 0) out vec3 color;

//Skybox Sampler
uniform samplerCube texture_sampler;


void main(){
	color = texture(texture_sampler, tex_direction).rgb;
}