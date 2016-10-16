#version 330

// Interpolated values from the vertex shaders
in vec2 uv;
in vec3 position_worldspace;
in vec3 normal_cameraspace;
in vec3 eye_direction_cameraspace;
in vec3 light_direction_cameraspace;

// Ouput data
layout(location = 0) out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D texture_sampler;

uniform vec3 light_position_worldspace;
uniform vec3 light_color;
uniform float light_intensity;

uniform vec3 ambient_material;
uniform vec3 specular_material;
uniform float shininess;

uniform vec3 picking_color;


void main(){
	// Material properties
	vec3 material_diffuse_color = texture(texture_sampler, uv).rgb;
	vec3 material_ambient_color = ambient_material * material_diffuse_color;
	vec3 material_specular_color = specular_material;

	// Distance to the light
	float distance = length(light_position_worldspace - position_worldspace); //magnitude of the vector

	// Normal of the computed fragment, in camera space
	vec3 n = normalize(normal_cameraspace);
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize(light_direction_cameraspace);
		
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cos_theta = clamp(dot(n,l), 0, 1); //clamp the value between 0 and 1. N dot L
	
	// Eye vector (towards the camera)
	vec3 e = normalize(eye_direction_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 r = reflect(-l,n);
	
	//vec3 H = normalize(LightDirection_cameraspace+EyeDirection_cameraspace);	
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cos_alpha = clamp(dot(e, r), 0, 1);
	
	//Linear Attenuation, based on distance.
	//Distance is divided by the max radius of the light which must be <= scale of the light mesh
	float attenuation = clamp((1.0f - distance/360.0),0.0,1.0); 
	
	color = 
	// Ambient : simulates indirect lighting
	(material_ambient_color +
	// Diffuse : "color" of the object
	(material_diffuse_color * light_color * light_intensity * cos_theta +
	// Specular : reflective highlight, like a mirror
	material_specular_color * light_color * light_intensity * pow(cos_alpha, shininess))*attenuation)+picking_color;
}
