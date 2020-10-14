#include "pch.h"
#include "graphics/graphics_types.h"
namespace thebanana {
	static graphics_api default_api = graphics_api::none;
	void set_default_graphics_api(graphics_api api) {
		default_api = api;
	}
	graphics_api get_default_graphics_api() {
		return default_api;
	}
}