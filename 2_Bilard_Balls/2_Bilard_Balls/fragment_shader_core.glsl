/* fragment shader - run for each pixel on the screen */

#version 440

const uint BOX = 0u;
const uint SPHERE = 1u;
const uint FLOOR = 2u;

in VS_OUT {
	vec3 vs_position;
	vec3 vs_color;
	vec2 vs_texcoord;
	vec3 vs_normal;
	vec4 FragPosLightSpace;
} fs_in;

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
	vec3 reflectDir = reflect( -lightDir, normalize(fs_in.vs_normal) );
	float spec = pow( max( dot( viewDir, reflectDir), 0.0), 32);

	return specularLightning * spec * ( DRAW_MODE == 1 ? vec3(1.f) : texture(specularTex, fs_in.vs_texcoord).rgb );
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMapTex, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}  

void main()
{
	if( DRAW_MODE == BOX )
	{
		fs_color = vec4(fs_in.vs_color, 1.f);
		//float depth = LinearizeDepth(gl_FragCoord.z)/far;
		//fs_color = vec4(vec3(depth), 1.0);
	}
	else if( DRAW_MODE == SPHERE || DRAW_MODE == FLOOR )
	{
		// Ambient 
		vec3 ambient = length(ambientLightning) > 0.f ? ambientLightning : vec3(1.f);

		// Diffuse
		vec3 diffuse = length(diffuseLightning) > 0.f ? calculateDiffuse( fs_in.vs_normal, lightPos0, fs_in.vs_position ) : vec3(1.f);

		// Specular
		vec3 specular = length(specularLightning) > 0.f ? calculateSpecular( cameraPosition, fs_in.vs_position, lightPos0 ) : vec3(1.f);

		// Calculate shadow 
		float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

		// Final Color
		vec3 final_color = ( ambient + + (1.0 - shadow) * (diffuse + specular )) * ( DRAW_MODE == SPHERE ? fs_in.vs_color : vec3(1.f) );
		
		fs_color = vec4(final_color, 1.f) * ( DRAW_MODE == SPHERE ? vec4(1.f) : texture(diffuseTex, fs_in.vs_texcoord) );
		
	}
	else
	{
		fs_color = vec4(1.f);
	}	
}