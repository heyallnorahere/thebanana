#pragma once
#include <string>
#include <glm/glm.hpp>
namespace opengl_shader_library {
	class uni {
	public:
		uni(unsigned int id);
		void _int(const std::string& name, int val);
		void _float(const std::string& name, float val);
		void vec3(const std::string& name, glm::vec3 val);
		void vec4(const std::string& name, glm::vec4 val);
		void mat4(const std::string& name, const glm::mat4& val, bool transpose = false);
	private:
		unsigned int id;
	};
	class shader {
	public:
		static void use(shader*);
		virtual uni& get_uniforms() = 0;
		virtual unsigned int get_id() = 0;
		virtual void reload() = 0;
		virtual ~shader() = 0;
	};
	class file_shader : public shader {
	public:
		file_shader(const std::string& name, bool geometry_shader = false);
		~file_shader();
		uni& get_uniforms();
		unsigned int get_id();
		void reload();
	private:
		std::string name;
		void create();
		unsigned int id;
		uni* uniforms;
		bool geometry_shader;
	};
#ifdef _WIN32
	class win32_resource_shader : public shader {
	public:
		win32_resource_shader(int vertex_id, int fragment_id, const std::string& resource_type = "SHADER", int geometry_id = 0);
		~win32_resource_shader();
		uni& get_uniforms();
		unsigned int get_id();
		void reload();
	private:
		int vertex_id, fragment_id, geometry_id;
		std::string resource_type;
		unsigned int id;
		uni* uniforms;
		void create();
	};
#endif
}