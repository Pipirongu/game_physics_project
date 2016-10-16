#version 330

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 vertexPosition_screenspace;
layout(location = 1) in vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

uniform mat4 projection;

void main()
{
	gl_Position =  projection * vec4(vertexPosition_screenspace.xy,0,1);
	
	// UV of the vertex. No special space for this one.
	UV = vertexPosition_screenspace.zw;
}

