#version 410 core

in vec2 f_uv;

uniform sampler2D tex;
uniform float scrollX;
uniform float scrollY;

out vec4 color;

void main () {
	float x = mod(f_uv.x - scrollX,  1.0);
	float y = mod(f_uv.y - scrollY,  1.0);
	color = texture(tex, vec2(x, y));
}