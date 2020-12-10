#pragma once
#include "banana_api.h"
namespace thebanana {
	class game;
	class shader_registry {
	public:
		BANANA_API void register_shader(const std::string& name, opengl_shader_library::shader* shader);
		BANANA_API opengl_shader_library::shader* get_shader(const std::string& name);
	private:
		std::map<std::string, std::unique_ptr<opengl_shader_library::shader>> m_shaders;
	};
}