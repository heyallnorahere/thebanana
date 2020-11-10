#include "pch.h"
#include "shader_registry.h"
namespace thebanana {
	void shader_registry::register_shader(const std::string& name, opengl_shader_library::shader* s) {
		this->m_shaders[name] = std::unique_ptr<opengl_shader_library::shader>(s);
	}
	opengl_shader_library::shader* shader_registry::get_shader(const std::string& name) {
		auto& s = this->m_shaders[name];
		return s.get();
	}
}