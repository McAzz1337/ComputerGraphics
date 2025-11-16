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
#include "model_data/generalMash.h"
#include "model_data/combinedMesh.h"
#include "shader.h"
#include "transform.h"
#include "model_data/texture.h"
#include "window.h"
#include "creation.h"


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
	//printf("moved: %f, %f\n", x, y);
	input.moveMouse({ (float) x, (float) y });
}
static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mod) {
	if (action == GLFW_PRESS) {
		input.pressMouseButton(button);
	} else if (action == GLFW_RELEASE) {
		input.releaseMouseButton(button);
	}
}




int meshIndex;
std::vector<GeneralMesh*> meshes;

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
	input.setAction(GLFW_KEY_RIGHT, [](float) {
		meshIndex = ++meshIndex % meshes.size();
		input.release(GLFW_KEY_RIGHT);
					});
	input.setAction(GLFW_KEY_LEFT, [](float) { 
		meshIndex = --meshIndex % meshes.size();
		input.release(GLFW_KEY_LEFT);
					});
	input.setAction(GLFW_KEY_P, [](float) { 
		meshes[meshIndex]->getMaterial().getShader().reloadFromDisk();
		input.release(GLFW_KEY_P);
					});
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
	/*
	Shader::loadShaders("shaders/shaders");
	Shader* simpleShader = Shader::getShader("shaders/shader");
	Shader* textureShader = Shader::getShader("shaders/tex_shader");
	Shader* bumpShader = Shader::getShader("shaders/bump_shader");
	bumpShader->setUniformi1("bump", 1);
	*/
#pragma endregion 

	Mesh mesh;
	Mesh plane;
	Mesh cube;
	Shader fractalShader("shaders/fractal_shader");
	Shader rayTracingShader("shaders/ray_tracer");
	{
		float resolution[2] = { 640.0f, 640.0f };
		//rayTracingShader.setUniform2f("resolution", resolution);
	}

	glm::mat4 trans1;
	glm::mat4 trans2;

	//textureShader->bind();
	float scrollTime = 10.0f;
	float scrollElapsed = 0.0f;

#pragma region GL_SETTINGS
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glfwSwapInterval(0);
#pragma endregion

	std::chrono::time_point last = std::chrono::steady_clock::now();
	float elapsed = 0.0f;

	createMeshes(meshes);
	meshIndex = meshes.size() - 1;
	while (window.shouldClose()) {
		std::chrono::time_point now = std::chrono::steady_clock::now();
		float delta = std::chrono::duration<float, std::milli>(now - last).count() / 1000.0f;
		scrollElapsed += delta;
		elapsed += delta;
		if (scrollElapsed / scrollTime > std::numbers::pi) {
			scrollElapsed = 0.0f;
		}
		last = now;
		float fps = 1.0f / delta;
		printf("%f.2\n", fps);
		glfwPollEvents();

		input.execute(delta);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 pv = cam.getProjectionView();

/*
		fractalShader.setUniformf1("time", elapsed);
		rayTracingShader.setMatrix3("viewMat", view);
		fractalShader.setMatrix3("viewMat", view);
*/
		glm::mat3 view = glm::mat3(cam.getView());
		GeneralMesh* mesh = meshes[meshIndex];
		CombinedMesh* cm = dynamic_cast<CombinedMesh*>(mesh);
		if (cm) {
			auto& meshes = cm->getMeshes();
			for (int i = 0; i < cm->getMeshCount(); i++) {
				Shader& shader = cm->getMaterial(i).getShader();
				shader.setUniformf1("time", elapsed);
				shader.setMatrix3("viewMat", view);
				shader.setMatrix4("model", glm::mat4(1.0f));
				float resolution[2] = { 640.0f, 640.0f };
				shader.setUniform2f("resolution", resolution);
				shader.setUniformf3("cameraPos", cam.getPos());
				cm->bind(pv * trans1, i);
				GL_CALL(glDrawElements(GL_TRIANGLES, cm->getIbo(i).getSize(), GL_UNSIGNED_INT, nullptr));
			}
		}
		else {
			Shader& shader = mesh->getMaterial().getShader();
			shader.setUniformf1("time", elapsed);
			//shader.setUniformi1("type", 2);
			shader.setMatrix3("viewMat", view);
			shader.setMatrix4("model", glm::mat4(1.0f));
			float resolution[2] = { 640.0f, 640.0f };
			shader.setUniform2f("resolution", resolution);
			shader.setUniformf3("cameraPos", cam.getPos());
			mesh->bind(pv * trans1);
			GL_CALL(glDrawElements(GL_TRIANGLES, mesh->getIbo().getSize(), GL_UNSIGNED_INT, nullptr));
		}

		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}
