#include "pch.h"
#include "shader_registry.h"
#include "graphics/shader.h"
namespace thebanana {
	shader_registry::shader_registry(game* g_game) : m_game(g_game) { }
	void shader_registry::register_shader(const std::string& name, graphics::shader* s) {
		this->m_shaders[name] = s;
	}
	void shader_registry::register_shader(const std::string& name, const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path) {
		this->register_shader(name, graphics::shader::create(vertex_path, fragment_path, geometry_path));
	}
	graphics::shader* shader_registry::get_shader(const std::string& name) {
		return this->m_shaders[name];
	}
	shader_registry::~shader_registry() {
		for (auto& s : this->m_shaders) {
			delete s.second;
		}
	}
}