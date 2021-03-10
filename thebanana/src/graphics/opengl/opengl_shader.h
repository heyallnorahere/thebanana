#pragma once
#include "graphics/shader.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			class opengl_shader : public shader {
			public:
				opengl_shader(const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path);
				opengl_shader(void* id);
				virtual ~opengl_shader() override;
				virtual void bind() override;
				virtual void unbind() override;
				virtual void* get_id() override;
				virtual void reload() override;
				virtual void uniform_int(const std::string& name, int val) override;
				virtual void uniform_float(const std::string& name, float val) override;
				virtual void uniform_vec3(const std::string& name, glm::vec3 val) override;
				virtual void uniform_vec4(const std::string& name, glm::vec4 val) override;
				virtual void uniform_mat4(const std::string& name, const glm::mat4& val, bool transpose = false) override;
			private:
				unsigned int m_id;
				std::string m_vertex_path, m_fragment_path, m_geometry_path;
				void init();
			};
		}
	}
}