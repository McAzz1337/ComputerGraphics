#pragma once
#include <variant>

#include <glm/vec4.hpp>
#include <optional>
#include <unordered_map>
#include "shader.h"
#include "texture.h"
#include "../io/assetImporter.h"


struct Material {
public:
	static const glm::vec4 DEFUALT_SPECULAR;
	static const glm::vec4 DEFUALT_DIFFUSE;
	static const glm::vec4 DEFUALT_AMBIENT;

	static const float DEFAULT_ROUGNESS;
	static const float DEFAULT_SPECULAR_STRENGTH;
	static const float DEFAULT_METALLIC;
private:


	glm::vec4 specular;
	glm::vec4 diffuse;
	glm::vec4 ambient;

	float specularStrength = 1.0f;
	float roughness = 0.0f;
	float metallic = 0.0f;

	Texture* tex = nullptr;
	Texture* tex1 = nullptr;
	Texture* bumpMap = nullptr;
	Texture* normalMap = nullptr;

	Shader shader;



public:
	Material();
	Material(const Material& m);
	Material(const glm::vec4& specular,
			 const glm::vec4& diffuse,
			 const glm::vec4& ambient,
			 float roughness,
			 float specularStength,
			 float metallic);

	~Material();

	void bind(const glm::mat4& projectionView) const;
	void assignAssets(std::unordered_map<assetimporter::AssetType, std::string> assets);
	void assignShader(Shader shader);

	inline Shader& getShader() { return shader;  }

	static void fromValues(Material& mat,
						   const glm::vec4 specular,
						   const glm::vec4 diffuse,
						   const glm::vec4 ambient,
						   const float roughness,
						   const float specularStrength,
						   const float metallic);

};