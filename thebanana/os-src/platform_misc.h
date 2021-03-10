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
		gl_context_t make_opengl_context(window_t window, int major_version, int minor_version);
		gl_context_t make_null_context(const gl_context_t& current);
		void destroy_context(const gl_context_t& context);
		void swap_buffers(const gl_context_t& context);
		void bind(const gl_context_t& context);
	}
}