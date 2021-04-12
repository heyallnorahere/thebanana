#pragma once
#include "banana_api.h"
namespace thebanana {
	namespace graphics {
		enum class graphics_api {
			none,
			opengl,
		};
		enum graphics_enum {
			FRAMEBUFFER,
			TEXTURE_2D,
			TEXTURE_CUBE_MAP,
			RGBA8,
			DEPTH24_STENCIL8,
			RGBA,
			DEPTH_STENCIL,
			DEPTH_COMPONENT,
			COLOR_ATTACHMENT,
			DEPTH_ATTACHMENT,
			DEPTH_STENCIL_ATTACHMENT,
			LINEAR,
			NEAREST,
			UNSIGNED_BYTE,
			UNSIGNED_INT,
			FLOAT,
			INT,
			BYTE,
			CLAMP_TO_BORDER,
			CLAMP_TO_EDGE,
			TEXTURE_CUBE_MAP_POSITIVE_X,
			NONE
		};
		BANANA_API void set_default_graphics_api(graphics_api api);
		BANANA_API graphics_api get_default_graphics_api();
		BANANA_API std::string get_backend_version(graphics_api api = graphics_api::none);
	}
}