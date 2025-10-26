#include "material.h"

const glm::vec4 Material::DEFUALT_SPECULAR = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
const glm::vec4 Material::DEFUALT_DIFFUSE = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
const glm::vec4 Material::DEFUALT_AMBIENT = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);

const float Material::DEFAULT_ROUGNESS = 0.5f;
const float Material::DEFAULT_SPECULAR_STRENGTH = 0.3f;
const float Material::DEFAULT_METALLIC = 0.0f;

Material::Material() {

}

Material::Material(const Material& m)
	: specular(m.specular), diffuse(m.diffuse), ambient(m.ambient), roughness(m.roughness), specularStrength(m.specularStrength),
	metallic(m.metallic), tex(m.tex), tex1(m.tex1), bumpMap(m.bumpMap), normalMap(m.bumpMap), shader(m.shader) {
	printf("copied material\n");
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
void Material::bind(const glm::mat4& projectionView) const {

	shader->bind();
	shader->setUniformf4("ambientColor", ambient);
	shader->setUniformf4("diffuseColor", diffuse);
	shader->setUniformf4("specularColor", specular);
	shader->setUniformf1("roughness", roughness);
	shader->setUniformf1("specularStrength", specularStrength);
	shader->setUniformf1("metallic", metallic);
	shader->setMatrix4("mvp", projectionView);

	if (tex) tex->bind();
	if (tex1) tex1->bind(1);
}

void Material::assignAssets(std::unordered_map<assetimporter::AssetType, std::string> textures) {
	auto it = textures.find(assetimporter::AssetType::DIFFUSE0);
	if (it != textures.end()) tex = new Texture(it->second);
	it = textures.find(assetimporter::AssetType::DIFFUSE1);
	if (it != textures.end()) tex1 = new Texture(it->second);
	it = textures.find(assetimporter::AssetType::BUMP_MAP);
	if (it != textures.end()) bumpMap = new Texture(it->second);
	it = textures.find(assetimporter::AssetType::NORMAL_MAP);
	if (it != textures.end()) normalMap = new Texture(it->second);
	it = textures.find(assetimporter::AssetType::SHADER);
	if (it != textures.end()) shader = new Shader(it->second);
	if (shader) shader->setUniformf1("tex1", 1);
}

void Material::assignShader(Shader* shader) {
	this->shader = shader;
	}

void Material::fromValues(Material& mat,
						  const glm::vec4 specular,
						  const glm::vec4 diffuse,
						  const glm::vec4 ambient,
						  const float roughness,
						  const float specularStrength,
						  const float metallic) {

	mat.specular = specular == glm::vec4(0.0f) ? DEFUALT_SPECULAR : specular;
	mat.diffuse = diffuse == glm::vec4(0.0f) ? DEFUALT_DIFFUSE : diffuse;
	mat.ambient = ambient == glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) ? DEFUALT_AMBIENT : ambient;
	mat.roughness = roughness < 0.0f ? DEFAULT_ROUGNESS : roughness;
	mat.specularStrength = specularStrength < 0.0f ? DEFAULT_SPECULAR_STRENGTH : specularStrength;
	mat.metallic = metallic < 0.0f ? DEFAULT_METALLIC : metallic;
}
