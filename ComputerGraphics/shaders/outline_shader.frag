#version 410 core

flat in int f_isOutline;
in vec3 f_norm;
in vec4 fragPos;
in float f_time;

uniform vec3 color = vec3(1.0);
uniform vec3 outlineColor = vec3(1.0, 0.0, 0.0);
uniform vec3 cameraPos;

uniform float ambientStrength = 0.3;
uniform vec3 ambientColor = vec3(1.0, 1.0, 1.0);
uniform vec3 diffuseColor = vec3(1.0, 1.0, 1.0);
uniform float specularStrength = 0.1;
uniform vec3 specularColor = vec3(1.0, 1.0, 1.0);

uniform float roughness = 0.5;

uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 lightPos = vec3(2.0, 1.0, 3.0);
uniform float metallic = 0.0;

out vec4 outColor;

const float PI = 22.0 / 7.0;

void main() {
	if (f_isOutline == 0) {
		vec3 lightDir = normalize(lightPos - fragPos.xyz);

		vec3 ambient = ambientStrength * ambientColor;

		float diff = max(dot(f_norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColor;

		vec3 viewDir = normalize(cameraPos - fragPos.xyz);
		vec3 reflectDir = reflect(-lightDir, f_norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
		vec3 specular = spec * lightColor;  
		outColor = vec4((ambient + diffuse + specular) * color, 1.0);
	} else {
		outColor = vec4(max(vec3(sin(outlineColor.x + f_time), cos(outlineColor.y + f_time), sin(outlineColor.z + f_time + PI * 1.3)), 0.4), 1.0);
	}
}