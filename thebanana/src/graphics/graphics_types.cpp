#include "pch.h"
#include "graphics/graphics_types.h"
namespace thebanana {
	namespace graphics {
		static graphics_api default_api = graphics_api::none;
		void set_default_graphics_api(graphics_api api) {
			default_api = api;
		}
		graphics_api get_default_graphics_api() {
			return default_api;
		}
		std::string get_backend_version(graphics_api api_) {
			graphics_api api = api_;
			if (api == graphics_api::none) {
				api = get_default_graphics_api();
			}
			switch (api) {
			case graphics_api::opengl:
				return (const char*)glGetString(GL_VERSION);
				break;
			}
			return std::string();
		}
	}
}