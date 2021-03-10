#pragma once
namespace thebanana {
	namespace platform_specific {
		using window_t = HWND;
		using cursor_t = HCURSOR;
		using module_t = HMODULE;
		enum class cursor_types {
			none = NULL,
			arrow = (size_t)IDC_ARROW,
		};
	}
}