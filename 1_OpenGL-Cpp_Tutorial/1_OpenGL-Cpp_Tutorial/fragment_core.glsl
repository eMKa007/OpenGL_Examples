/* fragment shader - run for each pixel on the screen */

#version 440

struct Material
{
	/* Colors and intensitives */
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	/* Texture */
	sampler2D diffuseTex;
	sampler2D specularTex;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

/* UNIFORMS */
uniform Material material;
uniform vec3 lightPos0;
uniform vec3 cameraPosition;

/* FUNCTIONS */
/* Ambient Light - effect like "if there is no light, you can see only ambient light" */
vec3 calculateAmbient( Material material )
{
	return material.ambient;
}

/* Diffuse Light 
 * posToLightDirVec - position to light normalized vector.
 * diffuse - dot product of given two vectors.
 */
vec3 calculateDiffuse( Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0 )
{
	vec3 posToLightDirVec = normalize(  lightPos0 - vs_position );
	float diffuse = clamp( dot(posToLightDirVec, vs_normal), 0, 1);
	vec3 diffuseFinal = material.diffuse * diffuse;

	return diffuseFinal;
}

/* Specular Lightning */
vec3 calculateSpecular( Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0, vec3 cameraPosition )
{
	vec3 lightToPosDirVec = normalize( vs_position - lightPos0 );
	vec3 reflectDirVec = normalize( reflect(lightToPosDirVec, normalize(vs_normal) ) );
	vec3 posToViewDirVec = normalize(  cameraPosition - vs_position );
	float SpecularConstant = pow( max( dot( posToViewDirVec, reflectDirVec), 0), 35);
	vec3 SpecularFinal = material.specular * SpecularConstant * texture(material.specularTex, vs_texcoord).rgb;

	return SpecularFinal;
}

void main()
{
	//fs_color = vec4(vs_color, 1.f);
	//fs_color = (texture( texture0, vs_texcoord ) + texture( texture1, vs_texcoord )) * vec4(vs_color, 1.f);
	
	vec3 ambientFinal = calculateAmbient( material );
	vec3 diffuseFinal = calculateDiffuse( material, vs_position, vs_normal, lightPos0);
	vec3 SpecularFinal = calculateSpecular( material, vs_position, vs_normal, lightPos0, cameraPosition);

	/* Attenuation */

	/* FINAL LIGHT */
	fs_color = 
		texture(material.diffuseTex, vs_texcoord) /* vec4(vs_color,1.f)*/
		 * ( vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(SpecularFinal, 1.f));
}