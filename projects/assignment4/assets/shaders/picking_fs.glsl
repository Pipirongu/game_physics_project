#version 330

// Ouput data
layout(location = 0) out vec3 color;
layout(location = 1) out vec3 world_pos;

in vec3 world_position;
// Values that stay constant for the whole mesh.
uniform float picking_id;
 
void main(){
	color = vec3(picking_id,0,0);
	world_pos = world_position;
}
