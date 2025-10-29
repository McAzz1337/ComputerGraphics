#version 410 core
in vec2 f_uv;
in vec3 f_norm;
in float f_time;

uniform sampler2D tex;
uniform sampler2D tex1;

out vec4 color;

void main() {
	float scrollX = f_uv.x - f_time; 
	float scrollY =  f_uv.y + f_time;
	vec4 col0 = texture(tex, vec2(scrollX + sin(f_time), scrollY));
	vec4 col1 = texture(tex1, vec2(scrollX + sin(f_time) + f_time, scrollY)) ;
	color = mix(col0, col1, sin(f_time) * 0.7);
	//color = texture(tex, vec2(scrollX + sin(f_time), scrollY));
	//color += texture(tex1, vec2(scrollX + sin(f_time) + f_time, scrollY)) + 1.2;
}