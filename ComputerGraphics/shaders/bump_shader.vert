#version 450 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;


uniform mat4 mvp;
uniform mat4 model;
uniform mat3 viewMat;

out vec3 f_normal;
out vec2 f_uv;
out vec4 fragPos;
out mat3 f_viewMat;

void main() {

	gl_Position = mvp * vec4(pos, 1.0);
	fragPos = model * vec4(pos, 1.0);

	f_normal = viewMat * normal;
	f_uv = uv;
	f_viewMat = viewMat;
}