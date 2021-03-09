#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#ifdef OPENGL_VIEWPORT_LIB_BUILD
#define OPENGL_VIEWPORT_DLL_ENTRY __declspec(dllexport)
#else
#define OPENGL_VIEWPORT_DLL_ENTRY __declspec(dllimport)
#endif
class opengl_viewport {
public:
	// make sure to zero before using i.e. memset(&my_attribs, 0, sizeof(viewport_attribs));
	struct viewport_attribs {
		enum render_target {
			child_window = 0,
			passed_window = 1,
		};
		HWND window;
		// x, y, width, height - do not matter if rendering directly to the passed window (specified in "target")
		int x,
			y,
			width,
			height;
		int major_version,
			minor_version;
		// specifies what to render to (child window or passed window)
		render_target target;
	};
	OPENGL_VIEWPORT_DLL_ENTRY opengl_viewport(const viewport_attribs& attribs);
	OPENGL_VIEWPORT_DLL_ENTRY void clear();
	OPENGL_VIEWPORT_DLL_ENTRY void swap_buffers();
	OPENGL_VIEWPORT_DLL_ENTRY ~opengl_viewport();
	OPENGL_VIEWPORT_DLL_ENTRY static void use(opengl_viewport* viewport);
	OPENGL_VIEWPORT_DLL_ENTRY static LRESULT __stdcall window_proc(HWND window, unsigned int msg, WPARAM w_param, LPARAM l_param);
private:
	OPENGL_VIEWPORT_DLL_ENTRY void init(HWND window, void* context_attr);
	HDC dc;
	HWND window;
	HGLRC context;
	bool is_child;
};
#define PREVIOUS_VIEWPORT ((opengl_viewport*)1)
