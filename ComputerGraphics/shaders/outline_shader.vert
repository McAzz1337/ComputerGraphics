#version 410 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 norm;

uniform mat4 mvp;
uniform float thickness;
uniform int isOutline;
uniform float time;

flat out int f_isOutline;
out vec4 fragPos;
out vec3 f_norm;
out float f_time;

void main() {
	vec4 p;
	if (isOutline == 0) {
		p = mvp * vec4(pos, 1.0);
	} else {	
		p = mvp * vec4(pos + norm * thickness * (sin(time) * 0.4 + 0.6), 1.0);
	}

	gl_Position = p;
	f_isOutline = isOutline;
	f_norm = norm;
	fragPos = p;
	f_time = time;
}
