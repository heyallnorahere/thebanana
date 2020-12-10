#pragma once
#include "banana_api.h"
namespace thebanana {
	namespace graphics {
		enum class graphics_api {
			none,
			opengl,
		};
		BANANA_API void set_default_graphics_api(graphics_api api);
		BANANA_API graphics_api get_default_graphics_api();
		BANANA_API std::string get_backend_version(graphics_api api = graphics_api::none);
	}
}