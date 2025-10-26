#version 410 core
in vec2 f_uv;

out vec4 color;

uniform float time;

vec3 colorPalette(float t) {
	//[[0.168 0.500 0.758] [0.968 0.188 0.698] [1.048 1.000 1.000] [-0.042 0.258 0.667]]
	vec3 a = vec3(0.168, 0.500, 0.758);
	vec3 b = vec3(0.968, 0.188, 0.698);
	vec3 c = vec3(1.048, 1.000, 1.000);
	vec3 d = vec3(-0.042, 0.258, 0.667);

	return a * b*cos(6.28218*(c*t+d));
}

void main() {

	vec3 finalColor = vec3(0.0);
	
	vec2 uv = f_uv;
	for (int i = 0; i < 3; i++) {
		 uv = fract(uv * 2.0);
		float d = length(uv - 0.5);
		vec3 c = colorPalette(d * i + time);
		d = abs(sin(d * 8.0 + time) / 8.0);
		d = 0.02 / d;

		finalColor += c * d;
	}

	color = vec4(finalColor, 1.0);
}