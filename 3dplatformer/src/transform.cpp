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
}