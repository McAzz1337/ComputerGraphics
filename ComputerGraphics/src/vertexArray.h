#pragma once

#include"buffer.h"

class VertexArray {
	uint32_t handle;
	VertexBuffer vbo;
	IndexBuffer ibo;

public:
	VertexArray();
	VertexArray(VertexBuffer vbo, IndexBuffer ibo);

	void enableAttribPointers();
	void assignBuffers(VertexBuffer vbo, IndexBuffer ibo);
	void bind();

	inline VertexBuffer getVbo() {
		return vbo;
	}

	inline IndexBuffer getIbo() {
		return ibo;
	}

};

