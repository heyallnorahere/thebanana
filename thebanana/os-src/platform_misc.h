#pragma once
#ifdef BANANA_WINDOWS
#include "windows/windows_defs.h"
#else
#error no other platforms supported
#endif
namespace thebanana {
	namespace platform_specific {
		std::string get_module_name(module_t module);
		size_t get_last_os_error();
	}
}