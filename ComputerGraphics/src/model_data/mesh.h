#pragma once
#include "generalMash.h"
#include "vertexArray.h"
#include "texture.h"
#include  "../shader.h"
#include "material.h"


class Mesh: public GeneralMesh {

	VertexArray vao;
	IndexBuffer ibo;
	Material mat;


public:
	Mesh();
	Mesh(VertexArray vao, IndexBuffer ibo);
	Mesh(VertexBuffer vbo, IndexBuffer ibo);
	Mesh(const Mesh& other);
	~Mesh();

	void assignVertexArray(VertexArray vao, IndexBuffer ibo) override;
	void assignBuffers(VertexBuffer vbo, IndexBuffer ibo) override;
	void assignMaterial(Material mat) override;

	void bind(const glm::mat4& projectionView) const override;

	inline Material& getMaterial() override { return mat;  }
	VertexBuffer& getVbo() override;
	IndexBuffer& getIbo() override;
};