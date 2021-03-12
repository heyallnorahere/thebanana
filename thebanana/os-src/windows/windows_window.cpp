#include "pch.h"
#include "window.h"
#include "game.h"
#include "application_layer.h"
#include "util.h"
#define CLASS_NAME "banana_window"
namespace thebanana {
	namespace platform_specific {
		void setup_window() {
			WNDCLASS wc;
			ZeroMemory(&wc, sizeof(WNDCLASS));
			wc.lpfnWndProc = ::thebanana::game::wndproc;
			wc.lpszClassName = TEXT(CLASS_NAME);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.hInstance = get_current_module();
			assert(RegisterClass(&wc));
		}
		int window_loop(application_layer* app_layer) {
			MSG msg;
			ZeroMemory(&msg, sizeof(MSG));
			while (msg.message != WM_QUIT) {
				if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				app_layer->gameloop();
			}
			return static_cast<int>(msg.wParam);
		}
		window_t create_window(const std::string& title, size_t width, size_t height, game* g) {
			WNDCLASS wc;
			if (!GetClassInfo(get_current_module(), TEXT(CLASS_NAME), &wc)) {
				DWORD error = GetLastError();
				__debugbreak();
			}
			return CreateWindowA(CLASS_NAME, title.c_str(), WS_VISIBLE | WS_SYSMENU | WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, NULL, g);
		}
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
		void set_window_title(window_t window, const std::string& title) {
			SetWindowTextA(window, title.c_str());
		}
		void destroy_window(window_t window) {
			DestroyWindow(window);
		}
		window_t get_focus() {
			return GetFocus();
		}
	}
}