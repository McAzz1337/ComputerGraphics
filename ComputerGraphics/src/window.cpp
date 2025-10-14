#include <iostream>

#include "window.h"

Window::Window(const char* title, int width, int height) : width(width), height(height) {

	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		exit(1);
	}

	handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwMakeContextCurrent(handle);

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		exit(2);
	}

}

void Window::setKeyCallback(GLFWkeyfun func) const {
	glfwSetKeyCallback(handle, func);
}

void Window::setMouseCallback(GLFWmousebuttonfun func) const {
	glfwSetMouseButtonCallback(handle, func);
}

void Window::setMousePositionCallback(GLFWcursorposfun func) const {
	glfwSetCursorPosCallback(handle, func);
}

void Window::setMouseButtonCallback(GLFWmousebuttonfun func) const {
	glfwSetMouseButtonCallback(handle, func);
}

void Window::close() const {
	glfwSetWindowShouldClose(handle, 1);
}

bool Window::shouldClose() const {
	return glfwWindowShouldClose(handle) == 0;
}

void Window::swapBuffers() const {
	glfwSwapBuffers(handle);
}
