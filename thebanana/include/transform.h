#pragma once
namespace thebanana {
	class transform {
	public:
		transform(const glm::vec3& translation = glm::vec3(0.f), const glm::vec3& rotation = glm::vec3(0.f), const glm::vec3& scale = glm::vec3(1.f));
		transform(const glm::mat4& m);
#ifdef BANANA_BUILD
		transform(const aiMatrix4x4& m);
#endif
		transform& translate(glm::vec3 t);
		transform& translate(float x, float y, float z);
		transform& rotate(glm::vec3 values);
		transform& rotate(float x, float y, float z);
		transform& scale(glm::vec3 s);
		transform& scale(float x, float y, float z);
		transform& scale(float s);
		transform& move(glm::vec3 offset);
		const transform& operator=(const glm::mat4& m);
#ifdef BANANA_BUILD
		const transform& operator=(const aiMatrix4x4& m);
#endif
		glm::mat4 get_matrix() const;
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
		static void decompose_matrix(const glm::mat4& matrix, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
		static glm::mat4 to_matrix(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);
	private:
		glm::vec3 m_translation, m_rotation, m_scale;
	};
}