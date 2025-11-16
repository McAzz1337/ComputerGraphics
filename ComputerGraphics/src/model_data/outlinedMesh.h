#pragma once
#include "generalMash.h"
#include "vertexArray.h"
#include "texture.h"
#include  "../shader.h"
#include "material.h"

class OutlinedMesh : public GeneralMesh {

	VertexArray vao;
	IndexBuffer ibo;
	Material mat;
	Material outlineMat;

public:
	OutlinedMesh();
	OutlinedMesh(VertexArray vao, IndexBuffer ibo);
	OutlinedMesh(VertexBuffer vbo, IndexBuffer ibo);
	OutlinedMesh(const OutlinedMesh& other);
	~OutlinedMesh();


	void assignVertexArray(VertexArray vao, IndexBuffer ibo) override;
	void assignBuffers(VertexBuffer vbo, IndexBuffer ibo) override;
	void assignMaterial(Material mat) override;
	void assignOutlineMaterial(Material mat);

	void bind(const glm::mat4& projectionView) const override;
	void bindOutline(const glm::mat4& projectionView) const;

	inline Material& getMaterial() override { return mat;  }
	VertexBuffer& getVbo() override;
	IndexBuffer& getIbo() override;
};
