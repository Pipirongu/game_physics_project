#version 330

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex_position_modelspace;

// Output data ; will be interpolated for each fragment.
out vec3 tex_direction;

// Values that stay constant for the whole mesh.
uniform mat4 mvp;

void main(){
	vec4 mvp_pos = mvp * vec4(vertex_position_modelspace,1);   
	//Supply w as z to make sure z is always 1 so that the skybox will always fail the depth test
	//The skybox will then only be rendered wherer there are no models. As long as the skybox is rendered last
	gl_Position = mvp_pos.xyww;  
	
	//Cube sampler will decide which face to sample from based on the highest value component
	tex_direction = vertex_position_modelspace;
}
