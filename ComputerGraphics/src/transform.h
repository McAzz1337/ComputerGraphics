#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Transform {

	bool dirty = true;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 mat;

public:
	Transform();
	~Transform();

	void reset();

	void translate(const glm::vec3& t);
	void rotate(const glm::vec3& r);
	void scale(const glm::vec3& s);

	void update();

	operator const glm::mat4& ();

	const glm::mat4 getMat();

	inline const glm::vec3 forward() {

		if (dirty) {
			update();
		}
		return (const glm::vec3) mat[2];
	}

	inline const glm::vec3 up() {

		if (dirty) {
			update();
		}
		return (const glm::vec3) mat[1];
	}

	inline const glm::vec3 right() {

		if (dirty) {
			update();
		}
		return (const glm::vec3) mat[0];
	}

	inline const glm::vec3 getPos() {

		if (dirty) {
			update();
		}
		return (const glm::vec3) mat[3];
	}


	void print() const;

	static const glm::vec3 globalRight();

	static const glm::vec3 globalUp();

	static const glm::vec3 globalForward();
};

