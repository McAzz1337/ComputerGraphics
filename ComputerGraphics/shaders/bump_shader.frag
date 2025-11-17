#version 450 core

in vec3 f_normal;
in vec2 f_uv;
in vec4 fragPos;
in mat3 f_viewMat;


 uniform sampler2D tex;
 uniform sampler2D bump;

uniform float ambientStrength = 0.3;
uniform vec3 ambientColor = vec3(1.0, 1.0, 1.0);
uniform vec3 diffuseColor = vec3(1.0, 1.0, 1.0);
uniform float specularStrength = 0.1;
uniform vec3 specularColor = vec3(1.0, 1.0, 1.0);

uniform float roughness = 0.5;
uniform vec3 cameraPos = vec3(0.0, 0.0, 3.0);

uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 lightPos = vec3(0.0, 0.0, 0.0);
uniform float metallic = 0.0;

uniform float time;

out vec4 color;


void main(){
	vec3 n = texture(bump, f_uv).xyz;
	vec3 norm = normalize((n * 2.0 - 1.0));
	vec3 light = lightPos + (vec3(sin(time), 0.0, 1.0) * 3.0);
	light = f_viewMat * light;
	vec3 lightDir = normalize(light - fragPos.xyz);

    vec3 ambient = ambientStrength * ambientColor;

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(cameraPos - fragPos.xyz);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;  

	vec4 pixelColor = texture(tex, f_uv);

	vec3 result = (ambient + diffuse + specular) * pixelColor.xyz;

	color = vec4(result, 1.0);
}