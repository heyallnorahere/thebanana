#pragma once
#include "graphics/graphics_types.h"
namespace thebanana {
	namespace graphics {
		namespace opengl {
			graphics_enum get_target(unsigned int texture);
			unsigned int get_bound_framebuffer();
			unsigned int get_current_shader();
			void unbind_all_textures(const std::vector<graphics_enum>& types);
			void get_texture_data(void* texid, void* buffer);
			void get_texture_size(void* texid, size_t& width, size_t& height);
			graphics_enum to_enum(unsigned int e);
			unsigned int from_enum(graphics_enum e);
		}
	}
}