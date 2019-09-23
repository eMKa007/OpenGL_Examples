/* vertex shader - run for each vertex */

#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;

out vec3 vs_position;
out vec3 vs_color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	vs_position = vec4( ModelMatrix * vec4(vertex_position, 1.f)).xyz;	/* Keep world coordinates for further calculations */
	vs_color = vertex_color;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);	/* Multiply every point in vertex shader by ModelMatrix  - final position*/
}