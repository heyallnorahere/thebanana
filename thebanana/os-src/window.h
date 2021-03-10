#pragma once
#ifdef BANANA_WINDOWS
#include "windows/windows_defs.h"
#else
namespace thebanana {
	namespace platform_specific {
		using window_t = void*;
	}
}
#endif
namespace thebanana {
	namespace platform_specific {
		size_t get_width(window_t window);
		size_t get_height(window_t window);
		void get_size(window_t window, size_t& width, size_t& height);
		void destroy_window(window_t window);
		window_t get_focus();
	}
}