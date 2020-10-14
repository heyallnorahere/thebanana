#pragma once
namespace thebanana {
	enum class graphics_api {
		none,
		opengl,
	};
	void set_default_graphics_api(graphics_api api);
	graphics_api get_default_graphics_api();
}