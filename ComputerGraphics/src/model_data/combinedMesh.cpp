#include "combinedMesh.h"

CombinedMesh::CombinedMesh() {}

CombinedMesh::CombinedMesh(std::vector<Mesh*> meshes) : meshes(meshes) {}

CombinedMesh::~CombinedMesh() {}

void CombinedMesh::assignVertexArray(VertexArray vao, IndexBuffer ibo) {
	assert(false);
}

void CombinedMesh::assignVertexArray(VertexArray vao, IndexBuffer ibo, size_t index) {
	if (index >= meshes.size()) assert(false);
	meshes[index]->assignVertexArray(vao, ibo);
}

void CombinedMesh::assignBuffers(VertexBuffer vbo, IndexBuffer ibo) {
	assert(false);
}

void CombinedMesh::assignBuffers(VertexBuffer vbo, IndexBuffer ibo, size_t index) {
	if (index >= meshes.size()) assert(false);
	meshes[index]->assignBuffers(vbo, ibo);
}

void CombinedMesh::assignMaterial(Material mat) {
	assert(false);
}

void CombinedMesh::assignMaterial(Material mat, size_t index) {
	if (index >= meshes.size()) assert(false);
	meshes[index]->assignMaterial(mat);
}


void CombinedMesh::bind(const glm::mat4& projectionView) const {
	assert(false);
}

void CombinedMesh::bind(const glm::mat4& projectionView, size_t index) const {
	if (index >= meshes.size()) assert(false);
	meshes[index]->bind(projectionView);
}



VertexBuffer& CombinedMesh::getVbo() {
	assert(false);
	return meshes[0]->getVbo();
}

VertexBuffer& CombinedMesh::getVbo(size_t index) {
	if (index >= meshes.size()) assert(false);
	return meshes[index]->getVbo();
}

IndexBuffer& CombinedMesh::getIbo() {
	assert(false);
	return meshes[0]->getIbo();
}

IndexBuffer& CombinedMesh::getIbo(size_t index) {
	if (index >= meshes.size()) assert(false);
	return meshes[index]->getIbo();
}
