#version 410 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 norm;

uniform mat4 mvp;
uniform float time;

out vec2 f_uv;
out vec3 f_norm;
out float f_time;
out vec3 f_pos;

struct Sines {
	float s;
	float s1;
	float s2;
};

Sines calculateSines(vec3 pos, float time) {
	Sines sines;
	sines.s = sin(time + pos.x + pos.z);
	sines.s1 = sin(time + pos.x);
	sines.s2 = sin(time + pos.z);
	return sines;
}

float calculateHeight(Sines s) {
	return (sin(2 * s.s + s.s1) + s.s2) * 0.25;
}

vec3 calculateAdjacentNormal(vec4 adjacent) {
	if (adjacent.y > 0.0) {
		return adjacent.xyz;
	} else {
		return reflect(-(adjacent.xyz), vec3(0.0, 1.0, 0.0));
	}
}

vec3[4] calculateAdjacentNormals(vec4[4] adjacent) {
	vec3[4] normals;
	for (int i = 0; i < 4; i++) {
		normals[i] = calculateAdjacentNormal(adjacent[i]);	
	}
	return normals;
}

void main() {
	Sines s = calculateSines(pos, time);
	vec3 position = vec3(pos.x, calculateHeight(s), pos.z);
	vec4 p = mvp * vec4(position, 1.0);
	gl_Position = p;

	vec3 right = pos + vec3(0.1, 0.0, 0.0);
	right = vec3(right.x, calculateHeight(calculateSines(right, time)), right.z);
	vec3 left = pos - vec3(0.1, 0.0, 0.0);
	left = vec3(left.x, calculateHeight(calculateSines(left, time)), left.z);
	vec3 up = pos + vec3(0.0, 0.0, 0.1);
	up  = vec3(up.x, calculateHeight(calculateSines(up, time)), up.z);
	vec3 down = pos - vec3(0.0, 0.0, 0.1);
	down  = vec3(down.x, calculateHeight(calculateSines(down, time)), down.z);
	vec3 n0 = cross(right - position, up - position);
	vec3 n1 = cross(up - position, left - position);
	vec3 n2 = cross(left - position, down - position);
	vec3 n3 = cross(down - position, right - position);

	vec3 normal = normalize(n0 + n1 + n2 + n3);
	f_norm = normal;
	f_uv = uv;
	f_time = time;
	f_pos = p.xyz;
}