#pragma once
#include "vertexArray.h"
#include "../texture.h"
#include  "../shader.h"
#include "material.h"


class Mesh {
	VertexArray vao;
	IndexBuffer ibo;
	Texture tex;
	Texture bump;
	Texture tex1;
	Material mat;


public:
	Mesh();
	Mesh(std::string texturePath);
	Mesh(VertexArray vao, IndexBuffer ibo);
	Mesh(VertexArray vao, IndexBuffer ibo, std::string texturePath);
	Mesh(VertexBuffer vbo, IndexBuffer ibo);
	Mesh(VertexBuffer vbo, IndexBuffer ibo, std::string texturePath);
	~Mesh();

	void assignVertexArray(VertexArray vao, IndexBuffer ibo);
	void assignBuffers(VertexBuffer vbo, IndexBuffer ibo);
	void assignTexture(const std::string& path);
	void assignTexture1(const std::string& path);
	void assignBumpMap(const std::string& path);
	void assignMaterial(Material mat);

	void bind(Shader* shader) const;

	VertexBuffer& getVbo();
	IndexBuffer& getIbo();
};