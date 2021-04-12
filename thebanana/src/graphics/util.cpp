#include "pch.h"
#include "graphics/util.h"
#include "opengl/opengl_util.h"
namespace thebanana {
	namespace graphics {
		namespace util {
			unsigned int get_target(unsigned int texture) {
				graphics_api api = get_default_graphics_api();
				switch (api) {
				case graphics_api::opengl:
					return opengl::get_target(texture);
					break;
				default:
					return 0;
					break;
				}
			}
			bool is_2d(unsigned int texture) {
				graphics_api api = get_default_graphics_api();
				switch (api) {
				case graphics_api::opengl:
					return opengl::is_2d(texture);
					break;
				default:
					return false;
					break;
				}
			}
			unsigned int get_bound_framebuffer() {
				graphics_api api = get_default_graphics_api();
				switch (api) {
				case graphics_api::opengl:
					return opengl::get_bound_framebuffer();
					break;
				default:
					return 0;
					break;
				}
			}
			unsigned int get_current_shader() {
				graphics_api api = get_default_graphics_api();
				switch (api) {
				case graphics_api::opengl:
					return opengl::get_current_shader();
					break;
				default:
					return 0;
					break;
				}
			}
			void unbind_all_textures(const std::vector<unsigned int>& types) {
				graphics_api api = get_default_graphics_api();
				switch (api) {
				case graphics_api::opengl:
					opengl::unbind_all_textures(types);
					break;
				default:
					break;
				}
			}
			void get_texture_data(void* texid, void* buffer) {
				graphics_api api = get_default_graphics_api();
				switch (api) {
				case graphics_api::opengl:
					opengl::get_texture_data(texid, buffer);
					break;
				default:
					break;
				}
			}
			void get_texture_size(void* texid, size_t& width, size_t& height) {
				graphics_api api = get_default_graphics_api();
				switch (api) {
				case graphics_api::opengl:
					opengl::get_texture_size(texid, width, height);
					break;
				default:
					break;
				}
			}
		}
	}
}
