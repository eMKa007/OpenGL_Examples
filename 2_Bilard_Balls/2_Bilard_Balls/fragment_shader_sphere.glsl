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

vec3 calculateDiffuse( vec3 vs_normal, vec3 lightPos0, vec3 vs_position )
{
	vec3 norm = normalize(vs_normal);
	vec3 lightDir = normalize( lightPos0 - vs_position );

	return diffuseLightning * max( dot(norm,lightDir), 0.0);
}

vec3 calculateSpecular( vec3 cameraPosition, vec3 vs_position, vec3 lightPos0 )
{
	vec3 viewDir = normalize( cameraPosition - vs_position );
	vec3 lightDir = normalize( lightPos0 - vs_position );
	vec3 reflectDir = reflect( -lightDir, normalize(vs_normal) );
	float spec = pow( max( dot( viewDir, reflectDir), 0.0), 32);

	return specularLightning*spec;
}

void main()
{
	// Ambient 
	vec3 ambient = length(ambientLightning) > 0.f ? ambientLightning : vec3(1.f);

	// Diffuse
	vec3 diffuse = length(diffuseLightning) > 0.f ? calculateDiffuse( vs_normal, lightPos0, vs_position ) : vec3(1.f);

	// Specular
	vec3 specular = length(specularLightning) > 0.f ? calculateSpecular( cameraPosition, vs_position, lightPos0 ) : vec3(1.f);

	// Final Color
	vec3 final_color = vs_color * ( ambient + diffuse + specular );
	fs_color = vec4(final_color, 1.f);
}