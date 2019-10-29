/* vertex shader - run for each vertex */

#version 440

const uint BOX = 0x00u;
const uint SPHERE = 0x01u;
const uint FLOOR = 0x02u;

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in vec3 vertex_normal;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out vec3 vs_normal;

/* UNIFORMS */
uniform int DRAW_MODE;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	if( DRAW_MODE == BOX )
	{
		vs_position = vec4( ModelMatrix * vec4(vertex_position, 1.f)).xyz;	/* Keep world coordinates for further calculations */
		vs_color = vertex_color;

		gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);	/* Multiply every point in vertex shader by ModelMatrix  - final position*/
	}
	else if( DRAW_MODE == SPHERE || DRAW_MODE == FLOOR )
	{	
		vs_position = vec4( ModelMatrix * vec4(vertex_position, 1.f)).xyz;	/* Keep world coordinates for further calculations */
		vs_color = vertex_color;
		vs_texcoord = vec2( tex_coord.x, tex_coord.y * -1.f);
		vs_normal = mat3(ModelMatrix) * vertex_normal;

		gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);	/* Multiply every point in vertex shader by ModelMatrix  - final position*/
	}
	else
	{
		vs_position = vec4( ModelMatrix * vec4(vertex_position, 1.f)).xyz;	/* Keep world coordinates for further calculations */
		vs_color = vertex_color;
		vs_texcoord = vec2( tex_coord.x, tex_coord.y * -1.f);
		vs_normal = mat3(ModelMatrix) * vertex_normal;

		gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);	/* Multiply every point in vertex shader by ModelMatrix  - final position*/
	}	
	
}