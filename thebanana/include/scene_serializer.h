#pragma once
#include "banana_api.h"
namespace thebanana {
#ifdef BANANA_BUILD
	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}
	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}
	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}
#endif
	class scene;
	class scene_serializer {
	public:
		BANANA_API scene_serializer(scene* scene);
		BANANA_API void serialize(const std::string& path);
		BANANA_API void deserialize(const std::string& path);
	private:
		scene* m_scene;
	};
}