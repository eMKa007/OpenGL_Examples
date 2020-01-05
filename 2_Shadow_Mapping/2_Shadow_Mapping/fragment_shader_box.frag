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

void main()
{
	fs_color = vec4(fs_in.vs_color, 1.f);
}