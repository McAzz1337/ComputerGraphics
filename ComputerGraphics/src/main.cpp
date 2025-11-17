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
#include "model_data/outlinedMesh.h"
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
		if (--meshIndex < 0) meshIndex = meshes.size() - 1;
		input.release(GLFW_KEY_LEFT);
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
		cam.rotate(glm::normalize(glm::vec3(y, x, 0.0f)) * 0.03f);
		input.clearMouseDelta();
						 });
#pragma endregion 

	glm::mat4 trans1;

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
	std::function<void(const Shader&, const glm::mat3&)> setUniforms = [&elapsed, &trans1, &cam](const Shader& shader, const glm::mat3& view) {
		shader.setUniformf1("time", elapsed);
		shader.setMatrix3("viewMat", view);
		shader.setMatrix4("model", trans1);
		shader.setUniformf3("cameraPos", cam.getPos());
		};
	std::function<void(GeneralMesh*)> render = [&setUniforms, &trans1, &cam](GeneralMesh* mesh) {
		const glm::mat4 pv = cam.getProjectionView();
		const glm::mat3 view = glm::mat3(cam.getView());
		if (CombinedMesh* cm = dynamic_cast<CombinedMesh*>(mesh)) {
			for (int i = 0; i < cm->getMeshCount(); i++) {
				const Shader& shader = cm->getMaterial(i).getShader();
				setUniforms(shader, view);
				cm->bind(pv * trans1, i);
				GL_CALL(glDrawElements(GL_TRIANGLES, cm->getIbo(i).getSize(), GL_UNSIGNED_INT, nullptr));
			}
		} else if (OutlinedMesh* om = dynamic_cast<OutlinedMesh*>(mesh)) {
			const Shader& shader = om->getMaterial().getShader();
			setUniforms(shader, view);
			om->bind(pv * trans1);
			GL_CALL(glDrawElements(GL_TRIANGLES, om->getIbo().getSize(), GL_UNSIGNED_INT, nullptr));
			glFrontFace(GL_CW);
			om->bindOutline(pv * trans1);
			GL_CALL(glDrawElements(GL_TRIANGLES, om->getIbo().getSize(), GL_UNSIGNED_INT, nullptr));
			glFrontFace(GL_CCW);
		} else {
			const Shader& shader = mesh->getMaterial().getShader();
			setUniforms(shader, view);
			mesh->bind(pv * trans1);
			GL_CALL(glDrawElements(GL_TRIANGLES, mesh->getIbo().getSize(), GL_UNSIGNED_INT, nullptr));
		}
		};
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
		//printf("%f.2\n", fps);
		glfwPollEvents();

		input.execute(delta);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		render(meshes[meshIndex]);


		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}
