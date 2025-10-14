#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {

	GLFWwindow* handle = nullptr;
	int width;
	int height;

public:
	Window(const char* title, int width, int height);

	void setKeyCallback(GLFWkeyfun func) const;
	void setMouseCallback(GLFWmousebuttonfun func) const;
	void setMousePositionCallback(GLFWcursorposfun func) const;
	void setMouseButtonCallback(GLFWmousebuttonfun func) const;

	void close() const;
	bool shouldClose() const;
	void swapBuffers() const;

	inline float getAspectRatio() const {
		return (float) width / (float) height;
	}
	
	inline GLFWwindow* getHandle() const {
		return handle;
	}
	
	inline int getWidth() const {
		return width;
	}

	inline int getHeight() const {
		return height;
	}
};
