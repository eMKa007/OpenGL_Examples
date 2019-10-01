/* fragment shader - run for each pixel on the screen */

#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

/* UNIFORMS */
uniform vec3 lightPos0;
uniform vec3 cameraPosition;

uniform vec3 ambientLightning;
uniform vec3 diffuseLightning;
uniform vec3 specularLightning;

void main()
{
	// Ambient 
	vec3 ambient = ambientLightning;

	// Diffuse
	vec3 norm = normalize(vs_normal);
	vec3 lightDir = normalize( lightPos0 - vs_position );
	vec3 diffuse = diffuseLightning * max( dot(norm,lightDir), 0.0);

	// Specular
	vec3 viewDir = normalize( cameraPosition - vs_position );
	vec3 reflectDir = reflect( -lightDir, norm );
	float spec = pow( max( dot( viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularLightning * spec;

	// Final Color
	vec3 final_color = vs_color * ( ambient + diffuse + specular );
	fs_color = vec4(final_color, 1.f);
}