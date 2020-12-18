#include "pch.h"
#include "graphics/vertex_buffer.h"
#include "opengl/opengl_vbo.h"
namespace thebanana {
	namespace graphics {
		class dummy_vbo : public vertex_buffer {
		public:
			dummy_vbo(const data& d);
			virtual void bind() override;
			virtual void unbind() override;
			virtual void draw() override;
		};
		vertex_buffer::vertex_buffer(const data& d) {
			this->m_data = d;
		}
		vertex_buffer::~vertex_buffer() { }
		vertex_buffer* vertex_buffer::create(const data& d, graphics_api api_) {
			graphics_api api = api_;
			if (api == graphics_api::none) {
				api = get_default_graphics_api();
			}
			switch (api) {
			case graphics_api::none:
				return new dummy_vbo(d);
				break;
			case graphics_api::opengl:
				return new opengl::opengl_vbo(d);
				break;
			}
			return NULL;
		}
		dummy_vbo::dummy_vbo(const data& d) : vertex_buffer(d) { }
		void dummy_vbo::bind() { }
		void dummy_vbo::unbind() { }
		void dummy_vbo::draw() { }
	}
}