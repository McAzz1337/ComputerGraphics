#include <chrono>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <numbers>

#include "camera.h"
#include "gldebug.h"
#include "input.h"
#include "io/assetImporter.h"
#include "io/fileio.h"
#include "model_data/buffer.h"
#include "model_data/vertexArray.h"
#include "model_data/mesh.h"
#include "shader.h"
#include "transform.h"
#include "model_data/texture.h"
#include "window.h"


void printMat(const glm::mat4& mat, const char* name = "mat") {
	printf("%s:\n", name);
	for (int i = 0; i < 4; i++) {
		printf("%f %f %f %f\n", mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
	}
}


Input input;

static void keyCallback(GLFWwindow* window, int key, int scan, int action, int mod) {
	switch (action) {
		case GLFW_PRESS: input.press(key); break;
		case GLFW_REPEAT: input.hold(key); break;
		case GLFW_RELEASE: input.release(key); break;
	}
}

static void cursorPosCallback(GLFWwindow* window, double x, double y) {
	printf("moved: %f, %f\n", x, y);
	input.moveMouse({ (float) x, (float) y });
}
static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mod) {
	if (action == GLFW_PRESS) {
		input.pressMouseButton(button);
	} else if (action == GLFW_RELEASE) {
		input.releaseMouseButton(button);
	}
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

int main() {

	assetimporter::init();
	Window window("ComputerGraphics", 1440, 1080);
	window.setKeyCallback(keyCallback);
	window.setMousePositionCallback(cursorPosCallback);
	window.setMouseButtonCallback(mouseButtonCallback);


	Camera cam((float) std::numbers::pi / 3.0f, window.getAspectRatio(), 0.001f, 100.0f);
	cam.translate({ 0.0f, 0.0f, -3.0f });
	cam.setResetPoint(cam.getPos());

#pragma region INPUT
	input.init(&window);
	input.setAction(GLFW_KEY_W, [&cam](float delta) { cam.translate(cam.forward() * delta); });
	input.setAction(GLFW_KEY_S, [&cam](float delta) { cam.translate(-cam.forward() * delta); });
	input.setAction(GLFW_KEY_A, [&cam](float delta) { cam.translate(-cam.right() * delta); });
	input.setAction(GLFW_KEY_D, [&cam](float delta) { cam.translate(cam.right() * delta); });
	input.setAction(GLFW_KEY_LEFT_CONTROL, [&cam](float delta) { cam.translate(-cam.up() * delta); });
	input.setAction(GLFW_KEY_SPACE, [&cam](float delta) { cam.translate(cam.up() * delta); });
	input.setAction(GLFW_KEY_ESCAPE, [&window](float delta) { window.close(); });
	input.setAction(GLFW_KEY_R, [&cam](float delta) { cam.reset(); });
	input.setOnMouseMove([&cam](float delta, float x, float y) {
		if (std::abs(x) < 1e-3f && std::abs(y) < 1e-3f) return;
		cam.rotate(glm::vec3(y, x, 0.0f) * 0.01f);
		input.clearMouseDelta();
						 });
#pragma endregion 

#pragma region SHADERS
	Shader::loadShaders("shaders/shaders");
	Shader* simpleShader = Shader::getShader("shaders/shader");
	Shader* textureShader = Shader::getShader("shaders/tex_shader");
	Shader* bumpShader = Shader::getShader("shaders/bump_shader");
	bumpShader->setUniformi1("bump", 1);
#pragma endregion 

	Mesh mesh;
	Mesh plane;
	Mesh cube;
	Shader fractalShader("shaders/fractal_shader");
	Shader rayTracingShader("shaders/ray_tracer");
	{
		float resolution[2] = { 640.0f, 640.0f };
		rayTracingShader.setUniform2f("resolution", resolution);
	}
#pragma region MESH
	{
		std::vector<Vertex> verticies;
		std::vector<uint32_t> indicies;
		Material material;
		assetimporter::loadModel("models/sphere", "sphere.obj", verticies, indicies, material);
		//assetimporter::loadModel("models", "die.obj", verticies, indicies, textures, material);
		VertexBuffer vbo(verticies.data(), verticies.size());
		uint32_t inds[] = { 0, 1, 2, 0, 2, 3 };
		IndexBuffer ibo(indicies.data(), indicies.size());
		mesh.assignBuffers(vbo, ibo);
		mesh.assignMaterial(material);
	}
	/*
	{
		std::vector<Vertex> verteces;
		std::vector<uint32_t> indices;
		createPlane(verteces, indices, 1.0f, glm::rotate(glm::mat4(1.0f), (float) std::numbers::pi / 2.0f, { 1.0f, 0.0f, 0.0f}));
		VertexBuffer vbo(verteces.data(), verteces.size());
		IndexBuffer ibo(indices.data(), indices.size());
		plane.assignBuffers(vbo, ibo);
		Material mat;
		mat.assignShader(&fractalShader);
		plane.assignMaterial(mat);
	}
	*/
	{
		std::vector<Vertex> verteces;
		std::vector<uint32_t> indices;
		createCube(verteces, indices, 1.0f, glm::mat4(1.0f));
		VertexBuffer vbo(verteces.data(), verteces.size());
		IndexBuffer ibo(indices.data(), indices.size());
		cube.assignBuffers(vbo, ibo);
		Material mat;
		//mat.assignShader(&fractalShader);
		mat.assignShader(&rayTracingShader);
		cube.assignMaterial(mat);
	}
#pragma endregion

	glm::mat4 trans1;
	glm::mat4 trans2;

	textureShader->bind();
	float scrollTime = 10.0f;
	float scrollElapsed = 0.0f;

#pragma region GL_SETTINGS
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
#pragma endregion

	std::chrono::time_point last = std::chrono::steady_clock::now();
	float elapsed = 0.0f;

	while (window.shouldClose()) {
		std::chrono::time_point now = std::chrono::steady_clock::now();
		float delta = std::chrono::duration<float, std::milli>(now - last).count() / 1000.0f;
		scrollElapsed += delta;
		elapsed += delta;
		if (scrollElapsed / scrollTime > std::numbers::pi) {
			scrollElapsed = 0.0f;
		}
		last = now;
		glfwPollEvents();

		input.execute(delta);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 pv = cam.getProjectionView();
		mesh.bind(pv);
		//GL_CALL(glDrawElements(GL_TRIANGLES, mesh.getIbo().getSize(), GL_UNSIGNED_INT, nullptr));


		cube.bind(pv * trans1);
		fractalShader.setUniformf1("time", elapsed);
		rayTracingShader.setUniformf1("time", elapsed);
		rayTracingShader.setUniformi1("type", 2);
		glm::mat3 view = glm::mat3(cam.getView());
		rayTracingShader.setMatrix3("viewMat", view);
		//fractalShader.setMatrix3("viewMat", view);
		GL_CALL(glDrawElements(GL_TRIANGLES, cube.getIbo().getSize(), GL_UNSIGNED_INT, nullptr));


		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}
