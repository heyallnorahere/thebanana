#include "pch.h"
#include "transform.h"
namespace thebanana {
	transform::transform() {
		this->m = glm::mat4(1.f);
	}
	transform::transform(const glm::mat4& m) : m(m) { }
	transform::transform(const aiMatrix4x4& m) {
		memcpy(&this->m, &m, sizeof(glm::mat4));
	}
	transform transform::translate(glm::vec3 t) {
		this->m = glm::translate(this->m, t);
		return *this;
	}
	transform transform::translate(float x, float y, float z) {
		return this->translate(glm::vec3(x, y, z));
	}
	transform transform::rotate(float r, glm::vec3 axis) {
		this->m = glm::rotate(this->m, glm::radians(r), axis);
		return *this;
	}
	transform transform::rotate(float r, float x, float y, float z) {
		return this->rotate(r, glm::vec3(x, y, z));
	}
	transform transform::scale(glm::vec3 s) {
		this->m = glm::scale(this->m, s);
		return *this;
	}
	transform transform::scale(float x, float y, float z) {
		return this->scale(glm::vec3(x, y, z));
	}
	transform transform::scale(float s) {
		return this->scale(glm::vec3(s));
	}
	transform transform::move(glm::vec3 offset) {
		glm::mat4 inverse = *this;
		inverse[3] = glm::vec4(0.f, 0.f, 0.f, inverse[3][3]);
		inverse = glm::inverse(inverse);
		inverse = glm::translate(inverse, offset);
		return this->translate(glm::vec3(inverse * glm::vec4(0.f, 0.f, 0.f, 1.f)));
	}
	const transform& transform::operator=(const glm::mat4& m) {
		this->m = m;
		return *this;
	}
	const transform& transform::operator=(const aiMatrix4x4& m) {
		memcpy(&this->m, &m, sizeof(glm::mat4));
		return *this;
	}
	const glm::mat4& transform::get_matrix() const {
		return this->m;
	}
	glm::mat4& transform::get_matrix() {
		return this->m;
	}
	transform::operator glm::mat4() {
		return this->m;
	}
	transform::operator glm::vec3() {
		return this->operator glm::vec4();
	}
	transform::operator glm::vec4() {
		return this->m * glm::vec4(0.f, 0.f, 0.f, 1.f);
	}
	transform transform::operator*(const glm::mat4& other) {
		return transform(this->m * other);
	}
	transform transform::operator*(const transform& other) {
		return *this * other.m;
	}
	transform transform::operator*(const aiMatrix4x4& other) {
		glm::mat4 glm_other;
		memcpy(&glm_other, &other, sizeof(glm::mat4));
		return *this * glm_other;
	}
	const transform& transform::operator*=(const glm::mat4& other) {
		*this = *this * other;
		return *this;
	}
	const transform& transform::operator*=(const transform& other) {
		*this = *this * other;
		return *this;
	}
	const transform& transform::operator*=(const aiMatrix4x4& other) {
		*this = *this * other;
		return *this;
	}
	void transform::decompose_matrix(const glm::mat4& matrix, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale) {
		translation = matrix[3];
		for (int i = 0; i < 3; i++) scale[i] = glm::length(glm::vec3(matrix[i]));
		// rotation matrix to quaternion to euler angles
		glm::mat4 rotation_matrix(1.f);
		for (int i = 0; i < 3; i++) rotation_matrix[i] = glm::vec4(glm::vec3(matrix[i]) / scale[i], 0.f);
		glm::quat q(rotation_matrix);
		float sinr_cosp = 2.f * (q.w * q.x + q.y * q.z);
		float cosr_cosp = 1.f - 2.f * (q.x * q.x + q.y * q.y);
		rotation.x = glm::degrees(std::atan2(sinr_cosp, cosr_cosp));
		float sinp = 2.f * (q.w * q.y - q.z * q.x);
		if (fabs(sinp) >= 1.f) rotation.y = copysign(static_cast<float>(M_PI) / 2.f, sinp);
		else rotation.y = asin(sinp);
		rotation.y = glm::degrees(rotation.y);
		float siny_cosp = 2.f * (q.w * q.z + q.x * q.y);
		float cosy_cosp = 1.f - 2.f * (q.y * q.y + q.z * q.z);
		rotation.z = glm::degrees(atan2(siny_cosp, cosy_cosp));
	}
	glm::mat4 transform::to_matrix(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale) {
		glm::mat4 rotation_matrix
			= glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f))
			* glm::rotate(glm::mat4(1.f), glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f))
			* glm::rotate(glm::mat4(1.f), glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
		return glm::translate(glm::mat4(1.f), translation) * rotation_matrix * glm::scale(glm::mat4(1.f), scale);
	}
}