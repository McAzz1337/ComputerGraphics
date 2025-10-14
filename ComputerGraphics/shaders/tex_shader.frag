#version 410 core

in vec2 f_uv;

uniform sampler2D tex;
uniform sampler2D tex1;
uniform float scrollX;
uniform float scrollY;

out vec4 color;

void main () {
	float x = f_uv.x - scrollX;
	float y = f_uv.y - scrollY;
	vec4 color1 = texture(tex, vec2(x, y));
	vec4 color2 = texture(tex1, vec2(-y, x));
	color = mix(color1, color2, 0.5);
}