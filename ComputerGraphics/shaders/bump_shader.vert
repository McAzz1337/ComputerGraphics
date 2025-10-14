#version 450 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

uniform mat4 mvp;
uniform mat4 model;
uniform float alpha = 1.0;

out vec3 f_normal;
out vec2 f_uv;
out vec4 fragPos;
out float f_alpha;

void main() {

	// Position auf dem Bildschirm berechnen
	gl_Position = mvp * vec4(pos, 1.0);
	// Pixelposition in World-space berechnen (fur Beleuchtung)
	fragPos = model * vec4(pos, 1.0);

	f_normal = normal;
	f_uv = uv;
	f_alpha = alpha;
}