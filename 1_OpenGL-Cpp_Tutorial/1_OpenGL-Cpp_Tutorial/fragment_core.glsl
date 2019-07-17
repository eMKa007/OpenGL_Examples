/* fragment shader - run for each pixel on the screen */

#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;

out vec4 fs_color;

uniform sampler2D texture0; /* Picker. Can sample color data from piceture */

void main()
{
	//fs_color = vec4(vs_color, 1.f);
	fs_color = texture( texture0, vs_texcoord ) * vec4(vs_color, 1.f);
}