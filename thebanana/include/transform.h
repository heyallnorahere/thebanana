#pragma once
#include "banana_api.h"
namespace thebanana {
	class transform {
	public:
		BANANA_API transform(const glm::vec3& translation = glm::vec3(0.f), const glm::vec3& rotation = glm::vec3(0.f), const glm::vec3& scale = glm::vec3(1.f));
		BANANA_API transform(const glm::mat4& m);
#ifdef BANANA_BUILD
		transform(const aiMatrix4x4& m);
#endif
		BANANA_API transform& translate(glm::vec3 t);
		BANANA_API transform& translate(float x, float y, float z);
		// remember: this is in radians
		BANANA_API transform& rotate(glm::vec3 values);
		// remember: this is in radians
		BANANA_API transform& rotate(float x, float y, float z);
		BANANA_API transform& scale(glm::vec3 s);
		BANANA_API transform& scale(float x, float y, float z);
		BANANA_API transform& scale(float s);
		BANANA_API transform& move(glm::vec3 offset);
		BANANA_API const transform& operator=(const glm::mat4& m);
#ifdef BANANA_BUILD
		const transform& operator=(const aiMatrix4x4& m);
#endif
		BANANA_API glm::mat4 get_matrix() const;
		BANANA_API glm::vec3 get_translation() const;
		// remember: this is in radians
		BANANA_API glm::vec3 get_rotation() const;
		BANANA_API glm::vec3 get_scale() const;
		BANANA_API void set_translation(glm::vec3 translation);
		// remember: this is in radians
		BANANA_API void set_rotation(glm::vec3 rotation);
		BANANA_API void set_scale(glm::vec3 scale);
		BANANA_API glm::vec3& translation();
		BANANA_API glm::vec3& rotation();
		BANANA_API glm::vec3& scale();
		BANANA_API operator glm::mat4();
		BANANA_API operator glm::vec3();
		BANANA_API operator glm::vec4();
		BANANA_API transform operator*(const glm::mat4& other);
		BANANA_API transform operator*(const transform& other);
#ifdef BANANA_BUILD
		transform operator*(const aiMatrix4x4& other);
#endif
		BANANA_API const transform& operator*=(const glm::mat4& other);
		BANANA_API const transform& operator*=(const transform& other);
#ifdef BANANA_BUILD
		const transform& operator*=(const aiMatrix4x4& other);
#endif
		BANANA_API static void decompose_matrix(const glm::mat4& matrix, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
		BANANA_API static glm::mat4 to_matrix(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);
	private:
		glm::vec3 m_translation;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;
	};
}