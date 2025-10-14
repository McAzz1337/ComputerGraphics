#include "transform.h"

#include <iostream>
#include <numbers>
#include <math.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr float TAU = std::numbers::pi * 2.0f;

Transform::Transform() {

}

Transform::~Transform() {

}

void Transform::translate(const glm::vec3& t) {

	pos += t;
	dirty = true;
}

void Transform::reset() {

	pos = { 0.0f, 0.0f, 0.0f };
	rot = { 0.0f, 0.0f, 0.0f };
	_scale = { 1.0f, 1.0f, 1.0f };

	dirty = true;
}

void Transform::rotate(const glm::vec3& r) {

	rot += r;

	for (int i = 0; i < 3; i++) {

		if (rot[i] >= TAU) {

			rot[i] -= TAU;
		} else if (rot[i] <= TAU) {

			rot[i] += TAU;
		}
	}

	dirty = true;
}


void Transform::rotateAxis(float angle, const glm::vec3& axis) {
	//rotation = glm::rotate(rotation, angle, axis);
	//rotation = glm::rotate(glm::mat4(1.0f), angle, axis) * rotation;
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, axis);
	rotation = rot * rotation; // <?? multiply on the LEFT
	dirty = true;
}


void Transform::scale(const glm::vec3& s) {

	_scale += s;
	dirty = true;
}

void Transform::update() {

	mat = glm::mat4(1.0f);

	//mat = glm::rotate(mat, rot.z, { 0.0f, 0.0f, 1.0f });
	//mat = glm::rotate(mat, rot.y, { 0.0f, 1.0f, 0.0f });
	//mat = glm::rotate(mat, rot.x, { 1.0f, 0.0f, 0.0f });

	//mat = glm::scale(mat, _scale);

	mat = glm::translate(pos) * rotation;
	//rotation = glm::mat4(1.0f);
	
	rotation[0] = glm::normalize(rotation[0]);
	rotation[1] = glm::normalize(rotation[1]);
	rotation[2] = glm::normalize(rotation[2]);

	dirty = false;
}

Transform::operator const glm::mat4& () {

	if (dirty) {

		update();
	}

	return mat;
}

const glm::mat4 Transform::getMat() {

	if (dirty) {

		update();
	}

	return mat;
}


void Transform::print() const {

	printf("---------------------------\n");
	for (int y = 0; y < 4; y++) {

		for (int x = 0; x < 4; x++) {

			printf("%f\t", mat[y][x]);
		}

		printf("\n");
	}
	printf("---------------------------\n");

}


const glm::vec3 Transform::globalRight() {

	return { 1.0f, 0.0f, 0.0f };
}

const glm::vec3 Transform::globalUp() {

	return { 0.0f, 1.0f, 0.0f };
}

const glm::vec3 Transform::globalForward() {

	return { 0.0f, 0.0f, 1.0f };
}