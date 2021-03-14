#pragma once
#include "banana_api.h"
#ifdef BANANA_WINDOWS
#include "windows/windows_defs.h"
#elif defined BANANA_LINUX
#include "linux/linux_defs.h"
#elif defined BANANA_MACOSX
#include "macosx/macosx_defs.h"
#else
#error no other platforms supported
#endif
namespace thebanana {
	class application_layer;
	class game;
	namespace platform_specific {
		BANANA_API void setup_window();
		BANANA_API int window_loop(application_layer* app_layer);
		BANANA_API window_t create_window(const std::string& title, size_t width, size_t height, game* g);
		BANANA_API size_t get_width(window_t window);
		BANANA_API size_t get_height(window_t window);
		BANANA_API void get_size(window_t window, size_t& width, size_t& height);
		BANANA_API void set_window_title(window_t window, const std::string& title);
		BANANA_API void destroy_window(window_t window);
		BANANA_API window_t get_focus();
	}
}