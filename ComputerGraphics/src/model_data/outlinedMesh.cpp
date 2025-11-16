#include "outlinedMesh.h"


OutlinedMesh::OutlinedMesh() {}
OutlinedMesh::OutlinedMesh(VertexArray vao, IndexBuffer ibo) : vao(vao), ibo(ibo) {}
OutlinedMesh::OutlinedMesh(VertexBuffer vbo, IndexBuffer ibo) : vao(VertexArray(vao)), ibo(ibo) {}
OutlinedMesh::OutlinedMesh(const OutlinedMesh& other) : vao(other.vao), ibo(other.ibo), mat(other.mat) {}
OutlinedMesh::~OutlinedMesh() {}


void OutlinedMesh::assignVertexArray(VertexArray vao, IndexBuffer ibo) {
	this->vao = vao;
	this->ibo = ibo;
}

void OutlinedMesh::assignBuffers(VertexBuffer vbo, IndexBuffer ibo) {
	vao.assignBuffer(vbo);
	this->ibo = ibo;
}

void OutlinedMesh::assignMaterial(Material mat) {
	this->mat = mat;
}

void OutlinedMesh::assignOutlineMaterial(Material mat) {
	outlineMat = mat;
}

void OutlinedMesh::bind(const glm::mat4& projectionView) const {
	vao.bind();
	ibo.bind();
	mat.getShader().setUniformi1("isOutline", 0);
	mat.bind(projectionView);
}

void OutlinedMesh::bindOutline(const glm::mat4& projectionView) const {
	vao.bind();
	ibo.bind();
	outlineMat.getShader().setUniformi1("isOutline", 1);
	outlineMat.bind(projectionView);
}

VertexBuffer& OutlinedMesh::getVbo() {
	return vao.getVbo();
}

IndexBuffer& OutlinedMesh::getIbo() {
	return ibo;
}
