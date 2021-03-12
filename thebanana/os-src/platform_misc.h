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
		BANANA_API std::string get_module_name(module_t module);
		BANANA_API std::string get_command_line();
		BANANA_API size_t get_last_os_error();
		BANANA_API gl_context_t make_opengl_context(window_t window, int major_version, int minor_version);
		BANANA_API gl_context_t make_null_context(const gl_context_t& current);
		BANANA_API void destroy_context(const gl_context_t& context);
		BANANA_API void swap_buffers(const gl_context_t& context);
		BANANA_API void bind(const gl_context_t& context);
		BANANA_API void init_imgui(window_t window);
		BANANA_API void newframe_imgui();
		BANANA_API void shutdown_imgui();
	}
}