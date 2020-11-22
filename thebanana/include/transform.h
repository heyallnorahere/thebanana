#pragma once
namespace thebanana {
	class transform {
	public:
		transform();
		transform(const glm::mat4& m);
#ifdef BANANA_BUILD
		transform(const aiMatrix4x4& m);
#endif
		transform translate(glm::vec3 t);
		transform translate(float x, float y, float z);
		transform rotate(float r, glm::vec3 axis);
		transform rotate(float r, float x, float y, float z);
		transform scale(glm::vec3 s);
		transform scale(float x, float y, float z);
		transform scale(float s);
		transform move(glm::vec3 offset);
		const transform& operator=(const glm::mat4& m);
#ifdef BANANA_BUILD
		const transform& operator=(const aiMatrix4x4& m);
#endif
		const glm::mat4& get_matrix() const;
		glm::mat4& get_matrix();
		operator glm::mat4();
		operator glm::vec3();
		operator glm::vec4();
		transform operator*(const glm::mat4& other);
		transform operator*(const transform& other);
#ifdef BANANA_BUILD
		transform operator*(const aiMatrix4x4& other);
#endif
		const transform& operator*=(const glm::mat4& other);
		const transform& operator*=(const transform& other);
#ifdef BANANA_BUILD
		const transform& operator*=(const aiMatrix4x4& other);
#endif
	private:
		glm::mat4 m;
	};
}