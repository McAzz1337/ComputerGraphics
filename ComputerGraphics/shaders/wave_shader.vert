#version 410 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 norm;

uniform mat4 mvp;
uniform float time;

out vec2 f_uv;
out vec3 f_norm;
out float f_time;


void main() {
	vec3 position = pos;
	float s = sin(time + pos.x + 2.0 + pos.z * 2.0);
	float s1 = sin(time + pos.x) * 2.0;
	float s2 = sin(time + pos.z) * 2.25;
	position.y += sin(time + pos.x + pos.z  + s1 + s2) * 0.1 + s1 * 0.2;
	//position.y += cos(time + sin(pos.z)) * 0.1;
	gl_Position = mvp * vec4(position, 1.0);
	//gl_Position = mvp * vec4(pos, 1.0);
	f_uv = uv;
	f_norm = norm;
	f_time = time;
}