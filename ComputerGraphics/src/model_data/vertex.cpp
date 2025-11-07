#include "vertex.h"

Vertex::Vertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 norm) 
	: pos(pos), uv(uv), norm(norm), tangent(glm::vec3(0.0f)), bitangent(glm::vec3(0.0f)) {

}

Vertex::Vertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 norm, glm::vec3 tangent, glm::vec3 bitangent) 
	: pos(pos), uv(uv), norm(norm), tangent(tangent), bitangent(bitangent) {

}