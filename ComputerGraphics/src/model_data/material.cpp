#include "material.h"



Material::Material() {

}

Material::Material(const Material& m)
	: specular(m.specular), diffuse(m.diffuse), ambient(m.ambient), roughness(m.roughness), specularStrength(m.specularStrength),
	metallic(m.metallic) {

}


Material::Material(const glm::vec4& specular,
				   const glm::vec4& diffuse,
				   const glm::vec4& ambient,
				   float roughness,
				   float specularStength,
				   float metallic)
	: specular(specular),
	diffuse(diffuse),
	ambient(ambient),
	roughness(roughness),
	specularStrength(specularStrength),
	metallic(metallic) {

}

Material::~Material() {

}

// Sendet Werte an das Shaderprogramm @arg shader
void Material::bind(Shader* shader) const {

	shader->bind();
	shader->setUniformf4("ambientColor", ambient);
	shader->setUniformf4("diffuseColor", diffuse);
	shader->setUniformf4("specularColor", specular);
	shader->setUniformf1("roughness", roughness);
	shader->setUniformf1("specularStrength", specularStrength);
	shader->setUniformf1("metallic", metallic);

}