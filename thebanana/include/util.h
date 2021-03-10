#pragma once
#ifdef BANANA_WINDOWS
extern "C" IMAGE_DOS_HEADER __ImageBase;
#endif
#include "script_module.h"
namespace thebanana {
	inline script_module::module_t get_current_module() {
#ifdef BANANA_WINDOWS
		return (script_module::module_t)&__ImageBase;
#endif
	}
}