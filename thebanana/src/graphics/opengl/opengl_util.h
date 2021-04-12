#pragma once
#include "graphics/graphics_types.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			unsigned int get_target(unsigned int texture);
			bool is_2d(unsigned int texture);
			unsigned int get_bound_framebuffer();
			unsigned int get_current_shader();
			void unbind_all_textures(const std::vector<unsigned int>& types);
			void get_texture_data(void* texid, void* buffer);
			void get_texture_size(void* texid, size_t& width, size_t& height);
		}
	}
}