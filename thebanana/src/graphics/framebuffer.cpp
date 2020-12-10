#include "pch.h"
#include "graphics/framebuffer.h"
#include "opengl/opengl_framebuffer.h"
namespace thebanana {
	namespace graphics {
		framebuffer::framebuffer(void* data) { }
		framebuffer::~framebuffer() { }
		framebuffer::attachment_map framebuffer::get_attachment_map() const {
			return this->m_attachment_map;
		}
		framebuffer* framebuffer::create(specification* spec, graphics_api api_) {
			graphics_api api = api_;
			if (api == graphics_api::none) {
				api = get_default_graphics_api();
			}
			switch (api) {
			case graphics_api::none:
				return new dummy_framebuffer(spec);
				break;
			case graphics_api::opengl:
				return new opengl::opengl_framebuffer(spec);
				break;
			}
			return NULL;
		}
		dummy_framebuffer::dummy_framebuffer(void* data) : framebuffer(data) { }
		void dummy_framebuffer::bind() { }
		void dummy_framebuffer::unbind() { }
		void dummy_framebuffer::set_size(int width, int height) { }
		void dummy_framebuffer::reload() { }
		void* dummy_framebuffer::get_id() { return NULL; }
		std::vector<framebuffer::attachment> dummy_framebuffer::get_attachments() {
			return std::vector<attachment>();
		}
	}
}