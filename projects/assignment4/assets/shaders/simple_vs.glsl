#version 330

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_normal_modelspace;

// Output data ; will be interpolated for each fragment.
out vec2 uv;
out vec3 position_worldspace;
out vec3 normal_cameraspace;
out vec3 eye_direction_cameraspace;
out vec3 light_direction_cameraspace;

// Values that stay constant for the whole mesh.
uniform mat4 mvp;
uniform mat4 view;
uniform mat4 model;
uniform mat4 modelview;
uniform vec3 light_position_worldspace;

void main(){
	//Output position of vertex in clip space
	gl_Position = mvp * vec4(vertex_position_modelspace, 1);

	// Position of the vertex, in worldspace : model * position
	position_worldspace = (model * vec4(vertex_position_modelspace,1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertex_position_cameraspace = (modelview * vec4(vertex_position_modelspace,1)).xyz;
	eye_direction_cameraspace = vec3(0,0,0) - vertex_position_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. For easier calculations later no need to get the opposite vector by then
	vec3 light_position_cameraspace = (view * vec4(light_position_worldspace,1)).xyz;
	light_direction_cameraspace = light_position_cameraspace + eye_direction_cameraspace;

	// Normal of the the vertex, in camera space
	normal_cameraspace = (modelview * vec4(vertex_normal_modelspace,0)).xyz; // Only correct if ModelMatrix does not scale the model non-uniformly! If so then use its inverse transpose.

	//UVs are sent to the fragment shader
	uv = vertex_uv;
}
