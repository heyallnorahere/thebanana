#include "pch.h"
#include "transform.h"
namespace thebanana {
	transform::transform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale) {
		this->m_translation = translation;
		this->m_rotation = rotation;
		this->m_scale = scale;
	}
	transform::transform(const glm::mat4& m) {
		decompose_matrix(m, this->m_translation, this->m_rotation, this->m_scale);
	}
	transform::transform(const aiMatrix4x4& m) {
		glm::mat4 glm_matrix;
		memcpy(&glm_matrix, &m, sizeof(glm::mat4));
		decompose_matrix(glm_matrix, this->m_translation,
			this->m_rotation, this->m_scale);
	}
	transform& transform::translate(glm::vec3 t) {
		glm::mat4 rotation = this->get_matrix();
		rotation[3] = glm::vec4(0.f, 0.f, 0.f, 1.f);
		this->m_translation += glm::vec3(rotation * glm::vec4(t, 1.f));
		return *this;
	}
	transform& transform::translate(float x, float y, float z) {
		return this->translate(glm::vec3(x, y, z));
	}
	transform& transform::rotate(glm::vec3 values) {
		this->m_rotation += values;
		return *this;
	}
	transform& transform::rotate(float x, float y, float z) {
		return this->rotate(glm::vec3(x, y, z));
	}
	transform& transform::scale(glm::vec3 s) {
		this->m_scale *= s;
		return *this;
	}
	transform& transform::scale(float x, float y, float z) {
		return this->scale(glm::vec3(x, y, z));
	}
	transform& transform::scale(float s) {
		return this->scale(glm::vec3(s));
	}
	transform& transform::move(glm::vec3 offset) {
		glm::mat4 inverse = *this;
		inverse[3] = glm::vec4(0.f, 0.f, 0.f, inverse[3][3]);
		inverse = glm::inverse(inverse);
		inverse = glm::translate(inverse, offset);
		return this->translate(glm::vec3(inverse * glm::vec4(0.f, 0.f, 0.f, 1.f)));
	}
	const transform& transform::operator=(const glm::mat4& m) {
		decompose_matrix(m, this->m_translation, this->m_rotation, this->m_scale);
		return *this;
	}
	const transform& transform::operator=(const aiMatrix4x4& m) {
		glm::mat4 glm_matrix;
		memcpy(&glm_matrix, &m, sizeof(glm::mat4));
		decompose_matrix(glm_matrix, this->m_translation, this->m_rotation, this->m_scale);
		return *this;
	}
	glm::mat4 transform::get_matrix() const {
		return to_matrix(this->m_translation, this->m_rotation, this->m_scale);
	}
	glm::vec3 transform::get_translation() const {
		return this->m_translation;
	}
	glm::vec3 transform::get_rotation() const {
		return this->m_rotation;
	}
	glm::vec3 transform::get_scale() const {
		return this->m_scale;
	}
	void transform::set_translation(glm::vec3 translation) {
		this->m_translation = translation;
	}
	void transform::set_rotation(glm::vec3 rotation) {
		this->m_rotation = rotation;
	}
	void transform::set_scale(glm::vec3 scale) {
		this->m_scale = scale;
	}
	transform::operator glm::mat4() {
		return this->get_matrix();
	}
	transform::operator glm::vec3() {
		return this->operator glm::vec4();
	}
	transform::operator glm::vec4() {
		return this->get_matrix() * glm::vec4(0.f, 0.f, 0.f, 1.f);
	}
	transform transform::operator*(const glm::mat4& other) {
		return transform(this->get_matrix() * other);
	}
	transform transform::operator*(const transform& other) {
		return *this * other.get_matrix();
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
		double sinr_cosp = 2.0 * static_cast<double>(q.w * q.x + q.y * q.z);
		double cosr_cosp = 1.0 - 2.0 * static_cast<double>(q.x * q.x + q.y * q.y);
		rotation.x = static_cast<float>(std::atan2(sinr_cosp, cosr_cosp));
		double sinp = 2.0 * static_cast<double>(q.w * q.y - q.z * q.x);
		if (abs(sinp) >= 1) rotation.y = static_cast<float>(copysign(M_PI / 2, sinp));
		else rotation.y = static_cast<float>(asin(sinp));
		double siny_cosp = 2.0 * static_cast<double>(q.w * q.z + q.x * q.y);
		double cosy_cosp = 1.0 - 2.0 * static_cast<double>(q.y * q.y + q.z * q.z);
		rotation.z = static_cast<float>(atan2(siny_cosp, cosy_cosp));
	}
	glm::mat4 transform::to_matrix(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale) {
		glm::mat4 rotation_matrix
			= glm::rotate(glm::mat4(1.f), rotation.x, glm::vec3(1.f, 0.f, 0.f))
			* glm::rotate(glm::mat4(1.f), rotation.y, glm::vec3(0.f, 1.f, 0.f))
			* glm::rotate(glm::mat4(1.f), rotation.z, glm::vec3(0.f, 0.f, 1.f));
		return glm::translate(glm::mat4(1.f), translation) * rotation_matrix * glm::scale(glm::mat4(1.f), scale);
	}
}