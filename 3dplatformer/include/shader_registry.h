#pragma once
namespace thebanana {
	class game;
	class shader_registry {
	public:
		void register_shader(const std::string& name, opengl_shader_library::shader* shader);
		opengl_shader_library::shader* get_shader(const std::string& name);
	private:
		std::map<std::string, std::unique_ptr<opengl_shader_library::shader>> m_shaders;
	};
}