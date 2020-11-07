#pragma once
#include "graphics/quad.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			class opengl_quad : public quad {
			public:
				opengl_quad(float width, float height, texture* tex, bool invert_uv);
				virtual ~opengl_quad() override;
				static void init_shader(const std::string& name);
			protected:
				virtual void api_render() override;
			private:
				void init_quad(bool invert_uv);
				void delete_quad();
				unsigned int m_vao, m_vbo, m_ebo;
				size_t m_index_count;
				static opengl_shader_library::shader* quad_shader;
			};
		}
	}
}