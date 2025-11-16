#version 410 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 norm;

uniform mat4 mvp;
uniform float time;
uniform int isFin;

out vec2 f_uv;

void main() {
	vec3 position = pos;
	float wiggle = sin(time * 1.2 + (position.x + 1.5)) * 0.5;
	position.z += wiggle;
	if (isFin == 1) {
		float s = pos.z > 0.0 ? 1.0 : -1.0;
		float sine = sin(time * 1.2 + position.y);
		float cosine = cos(time + position.y);
		position.y += sine * 0.3 * (pos.y + 0.7) ;
		if (s > 0.0) {
			position.z += max(sine, 0.0) * 0.4 * (pos.y + 0.7) * s + max(wiggle, 0.0) * 0.2;//max(sine, 0.0) * 0.3;
		} else {
			position.z += min(sine, 0.0) * 0.4 * (pos.y + 0.7) * s - min(wiggle, 0.0) * 0.2;
		}

	}
	gl_Position = mvp * vec4(position, 1.0);

	f_uv = uv;
}
