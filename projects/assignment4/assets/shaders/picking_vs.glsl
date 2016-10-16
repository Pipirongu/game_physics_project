#version 330
 
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex_position_modelspace;

// Values that stay constant for the whole mesh.
uniform mat4 mvp;
uniform mat4 model;
 
out vec3 world_position;

void main(){
 
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  mvp * vec4(vertex_position_modelspace,1);
	world_position =  (model * vec4(vertex_position_modelspace,1)).xyz;
}
