/* fragment shader - run for each pixel on the screen */

#version 440

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

	return specularLightning * spec * vec3(1.f);//( DRAW_MODE == 1 ? vec3(1.f) : texture(specularTex, fs_in.vs_texcoord).rgb );
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
	// Add small value to prevent shadow acne. this value is based on angle between normal and lightdir. 
	vec3 lightDir = normalize( lightPos0 - fs_in.vs_position );
	float bias = max(0.00001 * (1.0 - dot(fs_in.vs_normal, lightDir)), 0.00005);  // val between [0.00001, 0.00005]
    // check whether current frag pos is in shadow
    //float shadow = currentDepth-bias > closestDepth  ? 1.0 : 0.0;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMapTex, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMapTex, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;

	// To prevent draw shadow if scene is further than far plane of lights view
	if(projCoords.z > 1.0)
		shadow = 0.0;

    return shadow;
}  

void main()
{
	// Ambient 
	vec3 ambient = length(ambientLightning) > 0.f ? ambientLightning * fs_in.vs_color : vec3(0.5f);

	// Diffuse
	vec3 diffuse = length(diffuseLightning) > 0.f ? calculateDiffuse( fs_in.vs_normal, lightPos0, fs_in.vs_position ) : vec3(1.f);

	// Specular
	vec3 specular = length(specularLightning) > 0.f ? calculateSpecular( cameraPosition, fs_in.vs_position, lightPos0 ) : vec3(1.f);

	// Calculate shadow 
	float shadow = ShadowCalculation(fs_in.FragPosLightSpace);

	// Final Color
	vec3 final_color = ambient + (1.0 - shadow) * (diffuse + specular );
		
	fs_color = vec4(final_color, 1.f);// * texture(diffuseTex, fs_in.vs_texcoord);
}