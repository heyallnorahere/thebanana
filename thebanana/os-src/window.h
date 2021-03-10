#pragma once
#ifdef BANANA_WINDOWS
#include "windows/windows_defs.h"
#else
#error no other platforms supported
#endif
namespace thebanana {
	class application_layer;
	class game;
	namespace platform_specific {
		void setup_window();
		int window_loop(application_layer* app_layer);
		window_t create_window(const std::string& title, size_t width, size_t height, game* g);
		size_t get_width(window_t window);
		size_t get_height(window_t window);
		void get_size(window_t window, size_t& width, size_t& height);
		void destroy_window(window_t window);
		window_t get_focus();
	}
}