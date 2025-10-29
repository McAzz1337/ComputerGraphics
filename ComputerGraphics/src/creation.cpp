#include "creation.h"
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <numbers>

#include "transform.h"

void createSphere(std::vector<Mesh>& meshes) {
	std::vector<Vertex> verticies;
	std::vector<uint32_t> indicies;
	Material material;
	assetimporter::loadModel("models/sphere", "sphere.obj", verticies, indicies, material);
	VertexBuffer vbo(verticies.data(), verticies.size());
	uint32_t inds[] = { 0, 1, 2, 0, 2, 3 };
	IndexBuffer ibo(indicies.data(), indicies.size());
	Shader shader("shaders/tex_shader");
	material.assignShader(shader);
	Mesh mesh;
	mesh.assignBuffers(vbo, ibo);
	mesh.assignMaterial(material);
	meshes.push_back(mesh);
}

void createDie(std::vector<Mesh>& meshes) {
	std::vector<Vertex> verticies;
	std::vector<uint32_t> indicies;
	Material material;
	assetimporter::loadModel("models", "die.obj", verticies, indicies, material);
	VertexBuffer vbo(verticies.data(), verticies.size());
	uint32_t inds[] = { 0, 1, 2, 0, 2, 3 };
	IndexBuffer ibo(indicies.data(), indicies.size());
	Shader shader("shaders/bump_shader");
	material.assignShader(shader);
	Mesh mesh;
	mesh.assignBuffers(vbo, ibo);
	mesh.assignMaterial(material);
	meshes.push_back(mesh);
}

static void createPlane(std::vector<Vertex>& verteces, std::vector<uint32_t>& indices, float size, const glm::mat4& transform) {
	float side = size * 0.5f;
	glm::mat3 rot = (glm::mat3) transform;
	verteces.push_back(Vertex(glm::vec3(transform * glm::vec4(-side, -side, 0.0f, 1.0f)), { 0.0f, 0.0f }, rot * glm::vec3(0.0f, 0.0f, 1.0f)));
	verteces.push_back(Vertex(glm::vec3(transform * glm::vec4(side, -side, 0.0f, 1.0f)), { 1.0f, 0.0f }, rot * glm::vec3(0.0f, 0.0f, 1.0f)));
	verteces.push_back(Vertex(glm::vec3(transform * glm::vec4(side, side, 0.0f, 1.0f)), { 1.0f, 1.0f }, rot * glm::vec3(0.0f, 0.0f, 1.0f)));
	verteces.push_back(Vertex(glm::vec3(transform * glm::vec4(-side, side, 0.0f, 1.0f)), { 0.0f, 1.0f }, rot * glm::vec3(0.0f, 0.0f, 1.0f)));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
}

static void createCube(std::vector<Vertex>& verteces, std::vector<uint32_t>& indices, float size, const glm::mat4& transform) {
	float side = size * 0.5f;
	float angle90 = (float) std::numbers::pi * 0.5f;
	float angle180 = (float) std::numbers::pi;
	float angle125 = (float) std::numbers::pi * 1.5f;
	glm::mat4 up = glm::rotate(glm::translate(transform, Transform::globalUp() * side), angle125, Transform::globalRight());
	glm::mat4 down = glm::rotate(glm::translate(transform, -Transform::globalUp() * side), angle90, Transform::globalRight());
	glm::mat4 right = glm::rotate(glm::translate(transform, Transform::globalRight() * side), angle90, Transform::globalUp());
	glm::mat4 left = glm::rotate(glm::translate(transform, -Transform::globalRight() * side), angle125, Transform::globalUp());
	glm::mat4 front = glm::translate(transform, Transform::globalForward() * side);
	glm::mat4 back = glm::rotate(glm::translate(transform, -Transform::globalForward() * side), angle180, Transform::globalUp());


	auto create = [&verteces, &indices, &size](const glm::mat4& transform, uint32_t offset) {
		std::vector<uint32_t> temp;
		createPlane(verteces, temp, size, transform);
		for (int j = 0; j < temp.size(); j++) { temp[j] += 4 * offset; }
		indices.insert(indices.end(), temp.begin(), temp.end());
		};

	create(front, 0);
	create(up, 1);
	create(back, 2);
	create(down, 3);
	create(left, 4);
	create(right, 5);

}

void createFractalCube(std::vector<Mesh>& meshes) {
	std::vector<Vertex> verteces;
	std::vector<uint32_t> indices;
	createCube(verteces, indices, 1.0f, glm::mat4(1.0f));
	VertexBuffer vbo(verteces.data(), verteces.size());
	IndexBuffer ibo(indices.data(), indices.size());
	Mesh cube;
	cube.assignBuffers(vbo, ibo);
	Material mat;
	Shader shader("shaders/fractal_shader");
	mat.assignShader(shader);
	cube.assignMaterial(mat);
	meshes.push_back(cube);

}

void createRayTracerCube(std::vector<Mesh>& meshes) {
	std::vector<Vertex> verteces;
	std::vector<uint32_t> indices;
	createCube(verteces, indices, 1.0f, glm::mat4(1.0f));
	VertexBuffer vbo(verteces.data(), verteces.size());
	IndexBuffer ibo(indices.data(), indices.size());
	Mesh cube;
	cube.assignBuffers(vbo, ibo);
	Material mat;
	Shader shader("shaders/ray_tracer");
	shader.bind();
	mat.assignShader(shader);
	cube.assignMaterial(mat);
	meshes.push_back(cube);
}

void createSea(std::vector<Mesh>& meshes) {
	float quadSize = 0.1f;
	int dimension = 10;
	float offset = dimension * 0.5 * quadSize;
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (float) std::numbers::pi * -0.5f, { 1.0f, 0.0f, 0.0f });
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	for (int z = 0; z < dimension; z++) {
		for (int x = 0; x < dimension; x++) {
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), { x * quadSize - offset, -0.1f, z * quadSize - offset });
			std::vector<uint32_t> temp;
			createPlane(vertices, temp, quadSize, transform * rot);
		}
	}
	for (int i = 0; i < vertices.size() / 2 * 3; i += 4) {
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 2);

		indices.push_back(i);
		indices.push_back(i + 2);
		indices.push_back(i + 3);
	}

	Mesh sea;
	VertexBuffer vbo(vertices.data(), vertices.size());
	IndexBuffer ibo(indices.data(), indices.size());
	sea.assignBuffers(vbo, ibo);
	Material mat;
	Shader shader("shaders/wave_shader");
	shader.setUniformi1("tex1", 1);
	Texture tex("textures/albedo.png");
	Texture tex1("textures/albedo2.png");
	mat.assignShader(shader);
	mat.assignTex0(tex);
	mat.assignTex1(tex1);
	sea.assignMaterial(mat);
	meshes.push_back(sea);
}

void createMeshes(std::vector<Mesh>& meshes) {
	createSphere(meshes);
	createDie(meshes);
	createFractalCube(meshes);
	createRayTracerCube(meshes);
	createSea(meshes);
}
