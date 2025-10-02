#pragma once
#include "vertexArray.h"
#include "../texture.h"


class Mesh {
	VertexArray vao;
	IndexBuffer ibo;
	Texture tex;

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

	void bind() const;

	VertexBuffer& getVbo();
	IndexBuffer& getIbo();
};