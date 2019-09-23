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


void main()
{
	fs_color = vec4(0.f, 0.f, 0.f, 1.f);
}