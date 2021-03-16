#include "pch.h"
#include "platform_misc.h"
namespace thebanana {
	namespace platform_specific {
		std::string get_module_name(module_t module) {
			char buf[100];
			GetModuleFileNameA(module, buf, 100);
			return buf;
		}
		std::string get_command_line() {
			return GetCommandLineA();
		}
		size_t get_last_os_error() {
			return (size_t)GetLastError();
		}
		gl_context_t make_opengl_context(window_t window, int major_version, int minor_version) {
			gl_context_t result;
			result.dc = GetDC((HWND)window);
			result.context = NULL;
			PIXELFORMATDESCRIPTOR pfd;
			ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
			pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 32;
			pfd.cDepthBits = 32;
			pfd.cStencilBits = 8;
			pfd.iLayerType = PFD_MAIN_PLANE;
			int fmt = ChoosePixelFormat(result.dc, &pfd);
			if (!fmt) {
				return result;
			}
			bool succeeded = SetPixelFormat(result.dc, fmt, &pfd);
			if (!succeeded) {
				return result;
			}
			HGLRC temp = wglCreateContext(result.dc);
			wglMakeCurrent(result.dc, temp);
			unsigned int glew_error = glewInit();
			if (glew_error != GLEW_OK) {
				return result;
			}
			int attribs[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, major_version,
				WGL_CONTEXT_MINOR_VERSION_ARB, minor_version,
				WGL_CONTEXT_FLAGS_ARB, NULL,
				NULL
			};
			if (wglewIsSupported("WGL_ARB_create_context")) {
				result.context = wglCreateContextAttribsARB(result.dc, NULL, attribs);
				if (!result.context) {
					return result;
				}
				wglDeleteContext(temp);
				wglMakeCurrent(result.dc, result.context);
			}
			else {
				result.context = temp;
			}
			return result;
		}
		gl_context_t make_null_context(const gl_context_t& current) {
			return { current.dc, NULL };
		}
		void destroy_context(const gl_context_t& context) {
			wglDeleteContext(context.context);
		}
		void swap_buffers(const gl_context_t& context) {
			SwapBuffers(context.dc);
		}
		void bind(const gl_context_t& context) {
			wglMakeCurrent(context.dc, context.context);
		}
		void init_imgui(window_t window) {
			ImGui_ImplWin32_Init((HWND)window, wglGetCurrentContext());
		}
		void newframe_imgui() {
			ImGui_ImplWin32_NewFrame();
		}
		void shutdown_imgui() {
			ImGui_ImplWin32_Shutdown();
		}
	}
}