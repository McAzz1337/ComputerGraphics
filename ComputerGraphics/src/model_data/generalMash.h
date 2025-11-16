#pragma once

#include "vertexArray.h"
#include "texture.h"
#include  "../shader.h"
#include "material.h"

class GeneralMesh {

	public:	
	virtual ~GeneralMesh();

	virtual void assignVertexArray(VertexArray vao, IndexBuffer ibo) = 0;
	virtual void assignBuffers(VertexBuffer vbo, IndexBuffer ibo) = 0;
	virtual void assignMaterial(Material mat) = 0;

	virtual void bind(const glm::mat4& projectionView) const = 0;

	virtual inline Material& getMaterial() = 0; 
	virtual VertexBuffer& getVbo() = 0;
	virtual IndexBuffer& getIbo() = 0;
};
