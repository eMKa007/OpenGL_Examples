#version 430
in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 LightPosition;
uniform float LightIntensity;
uniform sampler2D texture_diffuse1;

layout( location = 0 ) out vec4 Ambient;
layout( location = 1 ) out vec4 DiffSpec;

void main() {
    vec3 toLight = normalize(LightPosition - Position);
	vec3 toV = normalize(vec3(-Position));
	vec3 r = normalize(reflect( -toLight, normalize(Normal )));

	float diffuse =  clamp( dot(toLight, Normal), 0, 1);
	float specular = pow( max( dot( toV, r), 0), 35);

	Ambient		= vec4(1.f) * LightIntensity * 0.1f ;
    DiffSpec	= vec4(1.f) * (diffuse+specular) * LightIntensity * 0.3f + Ambient;
}