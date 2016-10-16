#version 330

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D texture_sampler;
uniform vec3 text_color;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture_sampler, UV).r);
    color = vec4(text_color, 1.0) * sampled;
}