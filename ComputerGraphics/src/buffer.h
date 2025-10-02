#pragma once
#include <GL/glew.h>
#include <memory.h>
#include <iostream>

#include "vertex.h"


template <typename T, int BUFFER_TYPE>
class Buffer {
	uint32_t handle;
	size_t size;

public:
	Buffer() : size(0) {
		glGenBuffers(1, &handle);
		bind();
	}
	Buffer(const T* data, size_t size) {
		glGenBuffers(1, &handle);
		bind();
		bufferData(data, size);
	}

	void bind() { glBindBuffer(BUFFER_TYPE, handle); }

	void bufferData(const T* data) {
		this->size = sizeof(data);
		glBufferData(BUFFER_TYPE, sizeof(data) * sizeof(T), data, GL_STATIC_DRAW);
	}

	size_t getSize() { return size; }
};

typedef Buffer<Vertex, GL_ARRAY_BUFFER> VertexBuffer;
typedef Buffer<uint32_t, GL_ELEMENT_ARRAY_BUFFER> IndexBuffer;
