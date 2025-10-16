#include "camera.h"

#include <glm/gtx/transform.hpp>

Camera::Camera() {

}

Camera::Camera(float fov, float aspect, float near, float far) {

	projection = glm::perspective(fov, aspect, near, far);
}

Camera::~Camera() {

}

void Camera::setProjection(float fov, float aspect, float near, float far) {

	projection = glm::perspective(fov, aspect, near, far);
}

void Camera::translate(const glm::vec3& t) {
	//view.translate(t);
	view = glm::translate(view, t);
}

void Camera::rotate(const glm::vec3& r) {
	//view.rotate(r);
	view = glm::rotate(view, r.z, forward());
	view = glm::rotate(view, r.y, up());
	view = glm::rotate(view, r.x, right());
}

void Camera::reset() {
	//view.reset();
	view = glm::mat4(1.0f);
	//view.translate(resetPos);
	translate(resetPos);
	//view.rotate(resetRot);
	rotate(resetRot);
}

