#pragma once
#include "graphics/quad.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			class opengl_quad : public quad {
			public:
				opengl_quad(float width, float height, texture* tex);
				virtual ~opengl_quad() override;
				static void init_shader();
				static void destroy_shader();
			protected:
				virtual void api_render() override;
			private:
				void init_quad();
				void delete_quad();
				unsigned int m_vao, m_vbo, m_ebo;
				size_t m_index_count;
				static opengl_shader_library::shader* quad_shader;
			};
		}
	}
}