#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <numbers>
#include <chrono>

#include "window.h"
#include "input.h"
#include "shader.h"
#include "gldebug.h"
#include "buffer.h"
#include "vertexArray.h"
#include "camera.h"
#include "transform.h"
#include "gltexture.h"

void printMat(const glm::mat4& mat, const char* name = "mat") {
	printf("%s:\n", name);
	for (int i = 0; i < 4; i++) {
		printf("%f %f %f %f\n", mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
	}
}


Input input;

static void keyCallback(GLFWwindow* window, int key, int scan, int action, int mod) {
	if (action == GLFW_PRESS) {
		input.press(key);
	} else if (action == GLFW_REPEAT) {
		input.hold(key);
	} else if (action == GLFW_RELEASE) {
		input.release(key);
	}
}

static void cursorPosCallback(GLFWwindow* window, double x, double y) {
	input.moveMouse({ (float) x, (float) y });
}

int main() {
	Window window("ComputerGraphics", 1440, 1080);
	window.setKeyCallback(keyCallback);
	window.setMousePositionCallback(cursorPosCallback);
	Camera cam((float) std::numbers::pi / 3.0f, window.getAspectRatio(), 0.001f, 100.0f);
	cam.translate({ 0.0f, 0.0f, 1.0f });
	cam.setResetPoint(cam.getTransform().getPos());

	input.setWindow(&window);
	input.setAction(GLFW_KEY_W, [&cam](float delta) { cam.translate(-cam.getTransform().forward() * delta); });
	input.setAction(GLFW_KEY_S, [&cam](float delta) { cam.translate(cam.getTransform().forward() * delta); });
	input.setAction(GLFW_KEY_A, [&cam](float delta) { cam.translate(-cam.getTransform().right() * delta); });
	input.setAction(GLFW_KEY_D, [&cam](float delta) { cam.translate(cam.getTransform().right() * delta); });
	input.setAction(GLFW_KEY_LEFT_CONTROL, [&cam](float delta) { cam.translate(-Transform::globalUp() * delta); });
	input.setAction(GLFW_KEY_SPACE, [&cam](float delta) { cam.translate(Transform::globalUp() * delta); });
	input.setAction(GLFW_KEY_ESCAPE, [&window](float delta) { window.close(); });
	input.setAction(GLFW_KEY_R, [&cam](float delta) { cam.reset(); });
	input.setOnMouseMove([&cam](float delta, float x, float y) {
		input.clearMouseDelta();
		cam.rotate(glm::vec3(1.0f, 0.0f, 0.0f) * -y * delta);
		cam.rotate(glm::vec3(0.0f, 1.0f, 0.0f) * -x * delta);
						 });

	VertexBuffer vbo;
	IndexBuffer ibo;
	{
		Vertex verts[] = {
			Vertex({ -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f }),
			Vertex({ 0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f }),
			Vertex({ 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f }),
			Vertex({ -0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f }),
		};
		vbo.bufferData(verts);
		uint32_t inds[] = { 0, 1, 2, 0, 2, 3 };
		ibo.bufferData(inds);
	}

	VertexArray vao = VertexArray(vbo, ibo);

	Shader::loadShaders("shaders/shaders");
	Shader* s = Shader::getShader("shaders/shader");
	Shader* t = Shader::getShader("shaders/tex_shader");
	Transform trans1;
	Transform trans2;
	input.setAction(GLFW_KEY_UP, [&trans2](float delta) { trans2.translate(Transform::globalUp() * delta); });
	input.setAction(GLFW_KEY_DOWN, [&trans2](float delta) { trans2.translate(-Transform::globalUp() * delta); });
	input.setAction(GLFW_KEY_RIGHT, [&trans2](float delta) { trans2.translate(Transform::globalRight() * delta); });
	input.setAction(GLFW_KEY_LEFT, [&trans2](float delta) { trans2.translate(-Transform::globalRight() * delta); });

	GLTexture tex("textures/ha.png");
	t->bind();
	tex.bind();
	float scrollTime = 10.0f;
	float scrollElapsed = 0.0f;

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

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

		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 pv = cam.getProjectionView();
		auto mvp = pv *  trans1.getMat();
		printMat(mvp, "mvo");
		s->bind();
		s->setMatrix4("mvp", pv * trans2.getMat());
		GL_CALL(glDrawElements(GL_TRIANGLES, ibo.getSize(), GL_UNSIGNED_INT, nullptr));

		tex.bind();
		t->bind();
		float sin = std::sin(scrollElapsed);
		float cos = std::cos (scrollElapsed);
		trans1.translate(glm::vec3(-cos, sin, 0.0f) * 0.35f);
		trans1.translate(glm::vec3(1.5f, 0.0f, 0.0f));
		t->setUniformf1("scrollX", sin);
		t->setUniformf1("scrollY", cos);
		t->setMatrix4("mvp", pv *   trans1.getMat());
		trans1.reset();
		GL_CALL(glDrawElements(GL_TRIANGLES, ibo.getSize(), GL_UNSIGNED_INT, nullptr));

		window.swapBuffers();
	}

	return 0;
}
