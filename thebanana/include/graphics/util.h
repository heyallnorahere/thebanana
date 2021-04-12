#pragma once
#include "banana_api.h"
#include "graphics_types.h"
namespace thebanana {
	namespace graphics {
		namespace util {
			BANANA_API unsigned int get_target(unsigned int texture);
			BANANA_API bool is_2d(unsigned int texture);
			BANANA_API unsigned int get_bound_framebuffer();
			BANANA_API unsigned int get_current_shader();
			BANANA_API void unbind_all_textures(const std::vector<unsigned int>& types);
			BANANA_API void get_texture_data(void* texid, void* buffer);
			BANANA_API void get_texture_size(void* texid, size_t& width, size_t& height);
			BANANA_API graphics_enum to_enum(unsigned int e);
			BANANA_API unsigned int from_enum(graphics_enum e);
		}
	}
}
