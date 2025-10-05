#include "mesh.h"

Mesh::Mesh() {

}

Mesh::Mesh(std::string texturePath) : tex(Texture(texturePath)) {

}

Mesh::Mesh(VertexArray vao, IndexBuffer ibo) : vao(vao), ibo(ibo) {}

Mesh::Mesh(VertexArray vao, IndexBuffer ibo, std::string texturePath) : vao(vao), ibo(ibo), tex(Texture(texturePath)) {}

Mesh::Mesh(VertexBuffer vbo, IndexBuffer ibo) : vao(VertexArray(vbo)) {}

Mesh::Mesh(VertexBuffer vbo, IndexBuffer ibo, std::string texturePath) : vao(VertexArray(vbo)), tex(Texture(texturePath)) {}

Mesh::~Mesh() {}

void Mesh::assignVertexArray(VertexArray vao, IndexBuffer ibo) {
	this->vao = vao;
	this->ibo = ibo;
}

void Mesh::assignBuffers(VertexBuffer vbo, IndexBuffer ibo) {
	vao.assignBuffer(vbo);
	this->ibo = ibo;
}

void Mesh::assignTexture(const std::string& path) {
	tex.load(path);
}

void Mesh::bind() const {
	vao.bind();
	ibo.bind();
	tex.bind();
}

VertexBuffer& Mesh::getVbo() {
	return vao.getVbo();
}

IndexBuffer& Mesh::getIbo() {
	return ibo;
}
