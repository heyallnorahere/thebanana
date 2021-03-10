#pragma once
#ifdef BANANA_WINDOWS
#include "windows/windows_defs.h"
#else
namespace thebanana {
	namespace platform_specific {
		using cursor_t = void*;
	}
}
#endif
namespace thebanana {
	namespace platform_specific {
		void set_cursor(cursor_t c);
		cursor_t load_cursor(const char* resource);
		const char* make_cursor_location(cursor_types type);
	}
}