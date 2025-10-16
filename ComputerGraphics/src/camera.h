#pragma once

#include <glm/mat4x4.hpp>

#include "transform.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {

	glm::mat4 projection;
	//mutable Transform view;
	glm::mat4 view;
	glm::vec3 resetPos = glm::vec3(0.0f);
	glm::vec3 resetRot = glm::vec3(0.0f);

public:
	Camera();
	Camera(float fov, float aspect, float near, float far);
	~Camera();

	void setProjection(float fov, float aspect, float near, float far);

	void translate(const glm::vec3& t);
	void rotate(const glm::vec3& r);

	void reset();

	inline void setResetPoint(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f)) {
		resetPos = position;
		resetRot = rotation;
	}

	inline const glm::mat4 getProjection() const {
		return projection;
	}

	inline const glm::mat4 getProjectionView() const {
		//return projection * glm::inverse((const glm::mat4&) view);
		return projection * view;
	}

	/*
	inline Transform& getTransform() {
		return view;
	}
	*/

	inline const glm::mat4 getView() const {
		//return (const glm::mat4) view.getMat();
		return view;
	}

	inline const glm::vec3 getPos() const {
		return (const glm::vec3) view[3];
	}

	inline const glm::vec3 forward() {
		glm::mat4 inv = glm::inverse(view);
		return (const glm::vec3) inv [2];
	}

	inline const glm::vec3 up() {
		glm::mat4 inv = glm::inverse(view);
		return (const glm::vec3) inv[1];
	}

	inline const glm::vec3 right() {
		glm::mat4 inv = glm::inverse(view);
		return (const glm::vec3) inv[0];
	}
};