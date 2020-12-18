#include "pch.h"
#include "graphics/index_buffer.h"
#include "opengl/opengl_ebo.h"
namespace thebanana {
	namespace graphics {
		class dummy_ebo : public index_buffer {
		public:
			dummy_ebo(const data& d);
			virtual void bind() override;
			virtual void unbind() override;
			virtual void draw() override;
		};
		index_buffer::index_buffer(const data& d) {
			this->m_data = d;
		}
		index_buffer::~index_buffer() { }
		index_buffer* index_buffer::create(const data& d, graphics_api api_) {
			graphics_api api = api_;
			if (api == graphics_api::none) {
				api = get_default_graphics_api();
			}
			switch (api) {
			case graphics_api::none:
				return new dummy_ebo(d);
				break;
			case graphics_api::opengl:
				return new opengl::opengl_ebo(d);
				break;
			}
			return NULL;
		}
		dummy_ebo::dummy_ebo(const data& d) : index_buffer(d) { }
		void dummy_ebo::bind() { }
		void dummy_ebo::unbind() { }
		void dummy_ebo::draw() { }
	}
}