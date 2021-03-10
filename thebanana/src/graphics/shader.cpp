#include "pch.h"
#include "graphics/shader.h"
#include "opengl/opengl_shader.h"
namespace thebanana {
	namespace graphics {
		class dummy_shader : public shader {
		public:
			dummy_shader();
			virtual void bind() override;
			virtual void unbind() override;
			virtual void* get_id() override;
			virtual void reload() override;
			virtual void uniform_int(const std::string& name, int val) override;
			virtual void uniform_float(const std::string& name, float val) override;
			virtual void uniform_vec3(const std::string& name, glm::vec3 val) override;
			virtual void uniform_vec4(const std::string& name, glm::vec4 val) override;
			virtual void uniform_mat4(const std::string& name, const glm::mat4& val, bool transpose = false) override;
		};
		shader::shader(const std::string& vertex_path, const std::string& pixel_path, const std::string& geometry_path) {
			this->m_original = true;
		}
		shader::shader(void* id) {
			this->m_original = false;
		}
		shader::~shader() { }
		shader* shader::create(const std::string& vertex_path, const std::string& pixel_path, const std::string& geometry_path, graphics_api api_) {
			graphics_api api = api_;
			if (api == graphics_api::none) {
				api = get_default_graphics_api();
			}
			switch (api) {
			case graphics_api::opengl:
				return new opengl::opengl_shader(vertex_path, pixel_path, geometry_path);
				break;
			default:
				return new dummy_shader;
				break;
			}
			return NULL;
		}
		shader* shader::create(void* id, graphics_api api_) {
			graphics_api api = api_;
			if (api == graphics_api::none) {
				api = get_default_graphics_api();
			}
			switch (api) {
			case graphics_api::opengl:
				return new opengl::opengl_shader(id);
				break;
			default:
				return new dummy_shader;
				break;
			}
			return NULL;
		}
		dummy_shader::dummy_shader() : shader(NULL) { }
		void dummy_shader::bind() { }
		void dummy_shader::unbind() { }
		void* dummy_shader::get_id() { return NULL; }
		void dummy_shader::reload() { }
		void dummy_shader::uniform_int(const std::string& name, int val) { }
		void dummy_shader::uniform_float(const std::string& name, float val) { }
		void dummy_shader::uniform_vec3(const std::string& name, glm::vec3 val) { }
		void dummy_shader::uniform_vec4(const std::string& name, glm::vec4 val) { }
		void dummy_shader::uniform_mat4(const std::string& name, const glm::mat4& val, bool transpose) { }
	}
}