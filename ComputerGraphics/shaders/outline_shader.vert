#version 410 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 norm;

uniform mat4 mvp;
uniform float thickness;
uniform int isOutline;
uniform float time;

flat out int f_isOutline;

void main() {
	if (isOutline == 0) {
		gl_Position = mvp * vec4(pos, 1.0);
	} else {	
		gl_Position = mvp * vec4(pos + norm * thickness * (sin(time) * 0.4 + 0.6), 1.0);
	}

	f_isOutline = isOutline;
}
