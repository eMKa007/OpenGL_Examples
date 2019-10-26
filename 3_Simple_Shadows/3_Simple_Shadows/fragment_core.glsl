/* fragment shader - run for each pixel on the screen */

#version 440

in vec3 vs_position;
in vec3 vs_color;

out vec4 fs_color;

float near = 0.1;
float far = 15.0;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far)/(far + near - z * (far - near));
}

void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z)/far;
	fs_color = vec4(vec3(depth), 1.0);
	//fs_color = vec4(vs_color, 1.f);
}