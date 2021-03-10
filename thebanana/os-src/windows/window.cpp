#include "pch.h"
#include "window.h"
namespace thebanana {
	namespace platform_specific {
		size_t get_width(window_t window) {
			RECT r;
			GetClientRect(window, &r);
			return (size_t)abs(r.right - r.left);
		}
		size_t get_height(window_t window) {
			RECT r;
			GetClientRect(window, &r);
			return (size_t)abs(r.bottom - r.top);
		}
		void get_size(window_t window, size_t& width, size_t& height) {
			RECT r;
			GetClientRect(window, &r);
			width = (size_t)abs(r.right - r.left);
			height = (size_t)abs(r.bottom - r.top);
		}
		void destroy_window(window_t window) {
			DestroyWindow(window);
		}
		window_t get_focus() {
			return GetFocus();
		}
	}
}