#version 450 core

in vec3 f_normal;
in vec2 f_uv;
in vec4 fragPos;
in float f_alpha;


 uniform sampler2D tex;
 uniform sampler2D bump;

uniform vec3 ambientColor = vec3(1.0, 1.0, 1.0);
uniform vec3 diffuseColor = vec3(1.0, 1.0, 1.0);
uniform vec3 specularColor = vec3(1.0, 1.0, 1.0);

uniform float roughness = 0.5;
uniform float specularStrength = 0.1;
uniform vec3 cameraPos = vec3(0.0, 0.0, 3.0);

uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 lightPos = vec3(1.0, 3.0, 1.0);
uniform float ambientStrength = 0.3;
uniform float metallic = 0.0;

out vec4 color;


void main(){

	// Umgebungsbeleuchtung 
    vec3 ambient = ambientStrength * lightColor;
	
	// Normalisieren des Normalvektors & des Lichtvektors
	vec3 norm = normalize(texture(bump, f_uv).xyz );
	vec3 lightDir = normalize(lightPos - vec3(fragPos.xyz));

	// Diffuse Lichtreflektion
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Gebündelte Lichtreflektion
	vec3 viewDir = normalize(cameraPos - vec3(fragPos.xyz));
	vec3 reflectDir = normalize(reflect(lightDir, norm));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  

	// Pixel von Texture sampeln
	vec4 pixelColor = texture(tex, f_uv);

	//UmgebungsLicht und Lichtreflektion auf den Pixelfarbton anwenden
	vec3 result = (ambient + diffuse + specular) * pixelColor.xyz;

	color = vec4(result, f_alpha);
}