#pragma once
#include "banana_api.h"
#ifdef BANANA_WINDOWS
#include "windows/windows_defs.h"
#elif defined BANANA_LINUX
#include "linux/linux_defs.h"
#else
#error no other platforms supported
#endif
namespace thebanana {
	namespace platform_specific {
		BANANA_API void set_cursor(cursor_t c);
		BANANA_API cursor_t load_cursor(const char* resource);
		BANANA_API const char* make_cursor_location(cursor_types type);
	}
}