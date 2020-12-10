#include "pch.h"
#include "graphics/quad.h"
#include "graphics/texture.h"
#include "opengl/opengl_quad.h"
namespace thebanana {
	namespace graphics {
		quad::quad(float width, float height, texture* tex, bool invert_uv) : m_width(width), m_height(height), m_texture(tex) { }
		void quad::update() { }
		void quad::render() {
			this->m_texture->bind(0);
			this->api_render();
			this->m_texture->unbind();
		}
		quad* quad::create(float width, float height, texture* tex, bool invert_uv, graphics_api api_) {
			graphics_api api = api_;
			if (api == graphics_api::none) {
				api = get_default_graphics_api();
			}
			switch (api) {
			case graphics_api::none:
				return new dummy_quad(width, height, tex, invert_uv);
				break;
			case graphics_api::opengl:
				return new opengl::opengl_quad(width, height, tex, invert_uv);
				break;
			}
			return NULL;
		}
		void quad::init_opengl_shader(const std::string& name) {
			return opengl::opengl_quad::init_shader(name);
		}
		dummy_quad::dummy_quad(float width, float height, texture* tex, bool invert_uv) : quad(width, height, tex, invert_uv) { }
		void dummy_quad::api_render() { }
	}
}