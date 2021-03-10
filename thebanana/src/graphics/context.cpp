#include "pch.h"
#include "graphics/context.h"
#include "platform_misc.h"
#include "opengl/opengl_context.h"
namespace thebanana {
	namespace graphics {
		class dummy_context : public context {
		public:
			dummy_context(const window& w);
			virtual void swap_buffers() override;
			virtual void bind() override;
			virtual void unbind() override;
		};
		context::context(const window& w) {
			this->m_window = w;
		}
		context::~context() { }
		context* context::create(const window& w, void* data, graphics_api api_) {
			graphics_api api = api_;
			if (api == graphics_api::none) {
				api = get_default_graphics_api();
			}
			switch (api) {
			case graphics_api::opengl:
				return new opengl::opengl_context(w, (opengl_data*)data);
				break;
			default:
				return new dummy_context(w);
				break;
			}
			return NULL;
		}
		dummy_context::dummy_context(const window& w) : context(w) { }
		void dummy_context::swap_buffers() { }
		void dummy_context::bind() { }
		void dummy_context::unbind() { }
	}
}