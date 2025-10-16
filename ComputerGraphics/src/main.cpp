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
#include "texture.h"
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


int main() {

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
#pragma region MESH
	{
		std::vector<Vertex> verticies;
		std::vector<uint32_t> indicies;
		std::vector<std::string> textures;
		Material material;
		assetimporter::loadModel("models", "die.obj", verticies, indicies, textures, material);
		VertexBuffer vbo(verticies.data(), verticies.size());
		uint32_t inds[] = { 0, 1, 2, 0, 2, 3 };
		IndexBuffer ibo(indicies.data(), indicies.size());
		mesh.assignBuffers(vbo, ibo);
		mesh.assignTexture(textures[0]);
		mesh.assignTexture1(textures[1]);
		mesh.assignMaterial(material);
		mesh.bind(bumpShader);
	}
#pragma endregion

	glm::mat4 trans1;
	glm::mat4 trans2;
	//input.setAction(GLFW_KEY_UP, [&trans2](float delta) { trans2.translate(Transform::globalUp() * delta); });
	//input.setAction(GLFW_KEY_DOWN, [&trans2](float delta) { trans2.translate(-Transform::globalUp() * delta); });
	//input.setAction(GLFW_KEY_RIGHT, [&trans2](float delta) { trans2.translate(Transform::globalRight() * delta); });
	//input.setAction(GLFW_KEY_LEFT, [&trans2](float delta) { trans2.translate(-Transform::globalRight() * delta); });

	textureShader->bind();
	float scrollTime = 10.0f;
	float scrollElapsed = 0.0f;

#pragma region GL_SETTINGS
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
#pragma endregion

	std::chrono::time_point last = std::chrono::steady_clock::now();

	while (window.shouldClose()) {
		std::chrono::time_point now = std::chrono::steady_clock::now();
		float delta = std::chrono::duration<float, std::milli>(now - last).count() / 1000.0f;
		scrollElapsed += delta;
		if (scrollElapsed / scrollTime > std::numbers::pi) {
			scrollElapsed = 0.0f;
		}
		last = now;
		glfwPollEvents();

		input.execute(delta);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 pv = cam.getProjectionView();
		simpleShader->bind();
		simpleShader->setMatrix4("mvp", pv);// *trans2.getMat());
		//textureShader->bind();
		//textureShader->setMatrix4("mvp", pv * trans2.getMat());
		//bumpShader->bind();
		//bumpShader->setUniformf3("cameraPos ", cam.getTransform().getPos());
		//bumpShader->setMatrix4("mvp", pv * trans2.getMat());
		//bumpShader->setMatrix4("model", trans2.getMat());
		GL_CALL(glDrawElements(GL_TRIANGLES, mesh.getIbo().getSize(), GL_UNSIGNED_INT, nullptr));

		textureShader->bind();
		float sin = std::sin(scrollElapsed);
		float cos = std::cos(scrollElapsed);
		trans1 = glm::translate(trans1, glm::vec3(-cos, sin, 0.0f) * 0.35f);
		trans1 = glm::translate(trans1, glm::vec3(2.5f, 0.0f, 0.0f));
		textureShader->setUniformf1("scrollX", sin);
		textureShader->setUniformf1("scrollY", cos);
		textureShader->setMatrix4("mvp", pv * trans1);
		trans1= glm::mat4(1.0f);
		GL_CALL(glDrawElements(GL_TRIANGLES, mesh.getIbo().getSize(), GL_UNSIGNED_INT, nullptr));

		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}
