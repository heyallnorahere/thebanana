#pragma once
class transform {
public:
	transform();
	transform(const glm::mat4& m);
	transform(const aiMatrix4x4& m);
	transform translate(glm::vec3 t);
	transform translate(float x, float y, float z);
	transform rotate(float r, glm::vec3 axis);
	transform rotate(float r, float x, float y, float z);
	transform scale(glm::vec3 s);
	transform scale(float x, float y, float z);
	transform scale(float s);
	const transform& operator=(const glm::mat4& m);
	const transform& operator=(const aiMatrix4x4& m);
	const glm::mat4& get_matrix() const;
	glm::mat4& get_matrix();
	transform operator*(const glm::mat4& other);
	transform operator*(const transform& other);
	transform operator*(const aiMatrix4x4& other);
	const transform& operator*=(const glm::mat4& other);
	const transform& operator*=(const transform& other);
	const transform& operator*=(const aiMatrix4x4& other);
private:
	glm::mat4 m;
};