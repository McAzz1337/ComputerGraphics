#version 410 core
in vec3 f_pos;
in vec2 f_uv;
in vec3 f_norm;
in float f_time;

uniform sampler2D tex;
uniform sampler2D tex1;
uniform vec3 cameraPos;

out vec4 color;

vec3 ambient = vec3(0.3, 0.3, 0.3);

void main() {
	vec3 light = vec3(sin(f_time), 1.0, cos(f_time)) * 5.0;
	float scrollX = f_uv.x - f_time; 
	float scrollY =  f_uv.y + f_time;
	vec4 col0 = texture(tex, vec2(scrollX + sin(f_time), scrollY));
	vec4 col1 = texture(tex1, vec2(scrollX + sin(f_time) + f_time, scrollY)) ;

	vec3 toLight = normalize(light - f_pos);
	vec3 viewDir = normalize(cameraPos - f_pos);
	vec3 diff = vec3(0.5, 0.5, 0.5) * max(dot(viewDir, toLight), 0.0); 
	vec3 reflectedLight = reflect(toLight, f_norm);
	float spec = pow(max(dot(viewDir, reflectedLight), 0.0), 64.0);

	color = mix(col0, col1, sin(f_time) * 0.7) * vec4(diff + ambient + spec, 1.0);
	//color = texture(tex, vec2(scrollX + sin(f_time), scrollY));
	//color += texture(tex1, vec2(scrollX + sin(f_time) + f_time, scrollY)) + 1.2;
}