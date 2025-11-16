#pragma once
#include <vector>
#include "generalMash.h"
#include "mesh.h"


class CombinedMesh : public GeneralMesh {

	std::vector<Mesh*> meshes;

public:
	CombinedMesh();
	CombinedMesh(std::vector<Mesh*> meshes);
	~CombinedMesh();

	void assignVertexArray(VertexArray vao, IndexBuffer ibo) override;
	void assignVertexArray(VertexArray vao, IndexBuffer ibo, size_t index);
	void assignBuffers(VertexBuffer vbo, IndexBuffer ibo) override;
	void assignBuffers(VertexBuffer vbo, IndexBuffer ibo, size_t index);
	void assignMaterial(Material mat) override;
	void assignMaterial(Material mat, size_t index);

	void bind(const glm::mat4& projectionView) const override;
	void bind(const glm::mat4& projectionView, size_t index) const;

	inline std::vector<Mesh*>& getMeshes() { return meshes; }
	inline size_t getMeshCount() { return meshes.size(); }
	inline Material& getMaterial() override {  return meshes[0]->getMaterial(); };
	inline Material& getMaterial(size_t index) {  return meshes[index]->getMaterial(); };
	VertexBuffer& getVbo() override;
	VertexBuffer& getVbo(size_t index);
	IndexBuffer& getIbo() override;
	IndexBuffer& getIbo(size_t index);
};
