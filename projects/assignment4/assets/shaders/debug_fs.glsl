#version 330

// Ouput data
layout(location = 0) out vec3 color;

uniform vec3 wireframe_color;

void main(){
	color = wireframe_color;
}