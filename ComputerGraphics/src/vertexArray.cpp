#include "vertexArray.h"
#include "buffer.h"
#include <GL/glew.h>

VertexArray::VertexArray() {
	glGenVertexArrays(1, &handle);
	bind();
	vbo = VertexBuffer();
	ibo = IndexBuffer();
}

VertexArray::VertexArray(VertexBuffer vbo, IndexBuffer ibo) : vbo(vbo), ibo(ibo) {

	glGenVertexArrays(1, &handle);
	bind();
	vbo.bind();
	ibo.bind();

	enableAttribPointers();
}

void VertexArray::assignBuffers(VertexBuffer vbo, IndexBuffer ibo) {
	bind();
	vbo.bind();
	ibo.bind();
	this->vbo = vbo;
	this->ibo = ibo;
	enableAttribPointers();
}

void VertexArray::enableAttribPointers() {
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (const void*) 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (const void*) offsetof(Vertex, uv));
}

void VertexArray::bind() {
	glBindVertexArray(handle);
	ibo.bind();
}

