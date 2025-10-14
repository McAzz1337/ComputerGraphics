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
#include <chrono>

#include "window.h"
#include "input.h"
#include "shader.h"
#include "gldebug.h"
#include "model_data/buffer.h"
#include "model_data/vertexArray.h"
#include "model_data/mesh.h"
#include "camera.h"
#include "transform.h"
#include "texture.h"

#include "io/fileio.h"

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

#include "io/assetImporter.h"

int main() {

	Window window("ComputerGraphics", 1440, 1080);
	window.setKeyCallback(keyCallback);
	window.setMousePositionCallback(cursorPosCallback);
	window.setMouseButtonCallback(mouseButtonCallback);
	Camera cam((float) std::numbers::pi / 3.0f, window.getAspectRatio(), 0.001f, 100.0f);
	cam.translate({ 0.0f, 0.0f, 3.0f });
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
		if (x == 0 && y == 0) {
			return;
		}
		input.clearMouseDelta();
		cam.rotate(glm::vec3(-y, -x, 0.0f) * delta);
						 });

	Shader::loadShaders("shaders/shaders");
	Shader* simpleShader = Shader::getShader("shaders/shader");
	Shader* textureShader = Shader::getShader("shaders/tex_shader");
	Shader* bumpShader = Shader::getShader("shaders/bump_shader");
	bumpShader->setUniformi1("bump", 1);

	Mesh mesh;
	{
		Vertex verts[] = {
			Vertex({ -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }),
			Vertex({ 0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }),
			Vertex({ 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }),
			Vertex({ -0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }),
		};

		std::vector<Vertex> verticies;
		std::vector<uint32_t> indicies;
		std::vector<std::string> textures;
		Material material;
		assetimporter::loadModel("models", "die.obj", verticies, indicies, textures, material);
		printf("vertecies: %i\n", verticies.size());
		printf("indicies: %i\n", indicies.size());
		printf("textures\n");
		for (auto& t : textures) {
			printf("\t%s\n", t.c_str());
		}
		//VertexBuffer vbo(verts, 4);
		VertexBuffer vbo(verticies.data(), verticies.size());
		uint32_t inds[] = { 0, 1, 2, 0, 2, 3 };
		IndexBuffer ibo(indicies.data(), indicies.size());
		//IndexBuffer ibo(inds, 6);
		mesh.assignBuffers(vbo, ibo);
		mesh.assignTexture(textures[0]);
		mesh.assignTexture1(textures[1]);
		//mesh.assignBumpMap(textures[1]);
		mesh.assignMaterial(material);
		//mesh.assignTexture("textures/ha.png");
		mesh.bind(bumpShader);
	}

	Transform trans1;
	Transform trans2;
	trans2.setMode(1);
	input.setAction(GLFW_KEY_UP, [&trans2](float delta) { trans2.translate(Transform::globalUp() * delta); });
	input.setAction(GLFW_KEY_DOWN, [&trans2](float delta) { trans2.translate(-Transform::globalUp() * delta); });
	input.setAction(GLFW_KEY_RIGHT, [&trans2](float delta) { trans2.translate(Transform::globalRight() * delta); });
	input.setAction(GLFW_KEY_LEFT, [&trans2](float delta) { trans2.translate(-Transform::globalRight() * delta); });
	input.setOnMouseMoveButtonLeft([&trans2, &cam, &window](float delta, float dx, float dy) {
		if (abs(dx) < 1e-3f && abs(dy) < 1e-3f) return;
		if (glm::distance(input.getMouseClick(), input.getMousePos()) < 1e-3f) return;

		float width = window.getWidth();
		float height = window.getHeight();

		// Map screen position to virtual trackball
		auto mapToSphere = [&width, &height](const glm::vec2& v) -> glm::vec3 {
			float x = (2.0f * v.x - width) / width;
			float y = (height - 2.0f * v.y) / height; // flip Y
			float len2 = x * x + y * y;

			glm::vec3 result;
			if (len2 <= 1.0f) {
				result = glm::vec3(x, y, sqrtf(1.0f - len2));
			} else {
				float norm = 1.0f / sqrtf(len2);
				result = glm::vec3(x * norm, y * norm, 0.0f);
			}

			return glm::normalize(result); // <- normalize to avoid huge jumps
			};

		glm::vec3 clickPos = mapToSphere(input.getMouseClick());
		glm::vec3 currentPos = mapToSphere(input.getMousePos());

		// Axis in camera space
		glm::vec3 axis = glm::cross(clickPos, currentPos);
		if (glm::length(axis) < 1e-6f) return;
		axis = glm::normalize(axis);

		// Angle from dot product
		float dot = glm::clamp(glm::dot(clickPos, currentPos), -1.0f, 1.0f);
		float angle = glm::acos(dot);

		// Transform axis from camera space to world space
		glm::mat3 camRotation = glm::mat3(glm::transpose(cam.getView()));
		glm::vec3 worldAxis = glm::normalize(camRotation * axis);

		// Debug
		std::stringstream ss;
		ss << "----------------------------------\n";
		ss << "angle: " << angle << "\n";
		ss << "clickPos: " << glm::to_string(clickPos) << "\n";
		ss << "currentPos: " << glm::to_string(currentPos) << "\n";
		ss << "axis (cam): " << glm::to_string(axis) << "\n";
		ss << "axis (world): " << glm::to_string(worldAxis) << "\n";
		std::cout << ss.str();

		// Apply rotation in world space (camera-relative)
		trans2.rotateAxis(angle * delta * 1.0f, worldAxis);
		input.clearMouseDelta();
								   });

	//Texture tex("textures/ha.png");
	textureShader->bind();
	//tex.bind();
	float scrollTime = 10.0f;
	float scrollElapsed = 0.0f;

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 pv = cam.getProjectionView();
		//simpleShader->bind();
		//simpleShader->setMatrix4("mvp", mvp);
		textureShader->bind();
		textureShader->setMatrix4("mvp", pv * trans2.getMat());
		//bumpShader->bind();
		//bumpShader->setUniformf3("cameraPos ", cam.getTransform().getPos());
		//bumpShader->setMatrix4("mvp", pv * trans2.getMat());
		//bumpShader->setMatrix4("model", trans2.getMat());
		GL_CALL(glDrawElements(GL_TRIANGLES, mesh.getIbo().getSize(), GL_UNSIGNED_INT, nullptr));

		textureShader->bind();
		float sin = std::sin(scrollElapsed);
		float cos = std::cos(scrollElapsed);
		trans1.translate(glm::vec3(-cos, sin, 0.0f) * 0.35f);
		trans1.translate(glm::vec3(2.5f, 0.0f, 0.0f));
		textureShader->setUniformf1("scrollX", sin);
		textureShader->setUniformf1("scrollY", cos);
		textureShader->setMatrix4("mvp", pv * trans1.getMat());
		trans1.reset();
		GL_CALL(glDrawElements(GL_TRIANGLES, mesh.getIbo().getSize(), GL_UNSIGNED_INT, nullptr));

		window.swapBuffers();
	}

	return 0;
}
