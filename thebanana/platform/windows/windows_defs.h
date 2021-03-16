#pragma once
namespace thebanana {
	namespace platform_specific {
		using window_t = size_t;
		using cursor_t = HCURSOR;
		using module_t = HMODULE;
		struct gl_context_t {
			HDC dc;
			HGLRC context;
		};
		enum class cursor_types {
			none = NULL,
			arrow = (size_t)IDC_ARROW,
		};
	}
}