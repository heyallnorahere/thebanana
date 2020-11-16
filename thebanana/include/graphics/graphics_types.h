#pragma once
namespace thebanana {
	namespace graphics {
		enum class graphics_api {
			none,
			opengl,
		};
		void set_default_graphics_api(graphics_api api);
		graphics_api get_default_graphics_api();
		std::string get_backend_version(graphics_api api = graphics_api::none);
	}
}