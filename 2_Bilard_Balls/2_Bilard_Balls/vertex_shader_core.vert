/* vertex shader - run for each vertex */

#version 440

const uint BOX = 0x00u;
const uint SPHERE = 0x01u;
const uint FLOOR = 0x02u;

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in vec3 vertex_normal;

out VS_OUT {
	vec3 vs_position;
	vec3 vs_color;
	vec2 vs_texcoord;
	vec3 vs_normal;
	vec4 FragPosLightSpace;
} vs_out;

/* UNIFORMS */
uniform int DRAW_MODE;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 LightSpaceMatrix;

void main()
{
	if( DRAW_MODE == BOX )
	{
		vs_out.vs_position = vec4( ModelMatrix * vec4(vertex_position, 1.f)).xyz;	/* Keep world coordinates for further calculations */
		vs_out.vs_color = vertex_color;
		vs_out.FragPosLightSpace = LightSpaceMatrix * vec4(vs_out.vs_position, 1.0);

		gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);	/* Multiply every point in vertex shader by ModelMatrix  - final position*/
	}
	else if( DRAW_MODE == SPHERE || DRAW_MODE == FLOOR )
	{	
		vs_out.vs_position = vec4( ModelMatrix * vec4(vertex_position, 1.f)).xyz;	/* Keep world coordinates for further calculations */
		vs_out.vs_color = vertex_color;
		vs_out.vs_texcoord = vec2( tex_coord.x, tex_coord.y * -1.f);
		vs_out.vs_normal = mat3(ModelMatrix) * vertex_normal;
		vs_out.FragPosLightSpace = LightSpaceMatrix * vec4(vs_out.vs_position, 1.0);

		gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);	/* Multiply every point in vertex shader by ModelMatrix  - final position*/
	}
	else
	{
		vs_out.vs_position = vec4( ModelMatrix * vec4(vertex_position, 1.f)).xyz;	/* Keep world coordinates for further calculations */
		vs_out.vs_color = vertex_color;
		vs_out.vs_texcoord = vec2( tex_coord.x, tex_coord.y * -1.f);
		vs_out.vs_normal = mat3(ModelMatrix) * vertex_normal;
		vs_out.FragPosLightSpace = LightSpaceMatrix * vec4(vs_out.vs_position, 1.0);

		gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);	/* Multiply every point in vertex shader by ModelMatrix  - final position*/
	}	
	
}