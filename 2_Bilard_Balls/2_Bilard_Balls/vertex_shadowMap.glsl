#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in vec3 vertex_normal;

uniform mat4 gWVP;

out vec2 TexCoordOut;

void main()
{
	gl_Position = gWVP * vec4(vertex_position, 1.0);
	TexCoordOut = tex_coord;
}