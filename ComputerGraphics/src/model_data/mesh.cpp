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

void Mesh::assignTexture1(const std::string& path) {
	tex1.load(path);
}

void Mesh::assignBumpMap(const std::string& path) {
	bump.load(path);
}

void Mesh::assignMaterial(Material mat) {
	this->mat = mat;
}

void Mesh::bind(Shader* shader) const {
	vao.bind();
	ibo.bind();
	tex.bind();
	tex1.bind(1);
	//bump.bind(1);
	//mat.bind(shader);
}

VertexBuffer& Mesh::getVbo() {
	return vao.getVbo();
}

IndexBuffer& Mesh::getIbo() {
	return ibo;
}
