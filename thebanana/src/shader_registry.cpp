#include "pch.h"
#include "shader_registry.h"
namespace thebanana {
	shader_registry::shader_registry(game* g_game) : m_game(g_game) { }
	void shader_registry::register_shader(const std::string& name, opengl_shader_library::shader* s) {
		this->m_shaders[name] = s;
	}
	void shader_registry::register_shader(const std::string& name, int vertex, int fragment, int geometry) {
		this->register_shader(name, new opengl_shader_library::win32_resource_shader(vertex, fragment, "SHADER", geometry));
	}
	void shader_registry::register_shader(const std::string& name, const std::string& dir, bool geometry) {
		this->register_shader(name, new opengl_shader_library::file_shader(dir, geometry));
	}
	opengl_shader_library::shader* shader_registry::get_shader(const std::string& name) {
		return this->m_shaders[name];
	}
	shader_registry::~shader_registry() {
		for (auto& s : this->m_shaders) {
			delete s.second;
		}
	}
}