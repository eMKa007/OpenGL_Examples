/* fragment shader - run for each pixel on the screen */

#version 440

const uint BOX = 0u;
const uint SPHERE = 1u;
const uint FLOOR = 2u;

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

/* UNIFORMS */
uniform int DRAW_MODE;
uniform vec3 lightPos0;
uniform vec3 cameraPosition;

uniform vec3 ambientLightning;
uniform vec3 diffuseLightning;
uniform vec3 specularLightning;

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D shadowMapTex;

float near = 0.1;
float far = 50.0;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far)/(far + near - z * (far - near));
}

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

	return specularLightning * spec * ( DRAW_MODE == 1 ? vec3(1.f) : texture(specularTex, vs_texcoord).rgb );
}

void main()
{
	if( DRAW_MODE == BOX )
	{
		fs_color = vec4(vs_color, 1.f);
		//float depth = LinearizeDepth(gl_FragCoord.z)/far;
		//fs_color = vec4(vec3(depth), 1.0);
	}
	else if( DRAW_MODE == SPHERE || DRAW_MODE == FLOOR )
	{
		// Ambient 
		vec3 ambient = length(ambientLightning) > 0.f ? ambientLightning : vec3(1.f);

		// Diffuse
		vec3 diffuse = length(diffuseLightning) > 0.f ? calculateDiffuse( vs_normal, lightPos0, vs_position ) : vec3(1.f);

		// Specular
		vec3 specular = length(specularLightning) > 0.f ? calculateSpecular( cameraPosition, vs_position, lightPos0 ) : vec3(1.f);

		// Final Color
		vec3 final_color = ( ambient + diffuse + specular ) * ( DRAW_MODE == SPHERE ? vs_color : vec3(1.f) );
		
		fs_color = vec4(final_color, 1.f) * ( DRAW_MODE == SPHERE ? vec4(1.f) : texture(shadowMapTex, vs_texcoord) );
		
	}
	else
	{
		fs_color = vec4(1.f);
	}	
}