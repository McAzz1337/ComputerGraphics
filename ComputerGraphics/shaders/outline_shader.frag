#version 410 core

flat in int f_isOutline;

uniform vec3 color = vec3(1.0);
uniform vec3 outlineColor = vec3(1.0, 0.0, 0.0);

out vec4 outColor;

void main() {
	if (f_isOutline == 0) {
		outColor = vec4(color, 1.0);
	} else {
		outColor = vec4(outlineColor, 1.0);
	}
}