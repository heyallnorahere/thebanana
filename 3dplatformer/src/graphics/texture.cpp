#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "graphics/texture.h"
#include "graphics/opengl/opengl_texture.h"
namespace thebanana {
	namespace graphics {
		texture::texture(const data& d) : m_width(d.width), m_height(d.height), m_channels(d.channels) { }
		texture::~texture() { }
		int texture::get_width() const {
			return this->m_width;
		}
		int texture::get_height() const {
			return this->m_height;
		}
		int texture::get_channels() const {
			return this->m_channels;
		}
		unsigned int texture::get_id() const {
			return this->id;
		}
		texture* texture::create(const data& d, void* api_specific, graphics_api api_) {
			graphics_api api = api_;
			if (api == graphics_api::none) {
				api = get_default_graphics_api();
			}
			switch (api) {
			case graphics_api::none:
				return new dummy_texture(d);
				break;
			case graphics_api::opengl:
				return new opengl::opengl_texture(d, api_specific);
				break;
			}
			return NULL;
		}
		dummy_texture::dummy_texture(const data& d) : texture(d) { }
		void dummy_texture::bind() { }
		void dummy_texture::unbind() { }
		void dummy_texture::set_data(void* pixels) { }
	}
}