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
	
	view.translate(t);
}

void Camera::rotate(const glm::vec3& r) {

	view.rotate(r);
}

void Camera::reset() {
	view.reset();
	view.translate(resetPos);
	view.rotate(resetRot);
}

void Camera::printView() const {

	view.print();
}
