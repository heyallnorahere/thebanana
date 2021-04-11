#include "pch.h"
#include "platform_misc.h"
#include "window.h"
#include "glx_functions.h"
extern xcb_connection_t* connection;
extern xcb_screen_t* screen;
extern Display* display;
extern GLXFBConfig fb_config;
extern int visual_id;
namespace thebanana {
    namespace platform_specific {
        std::string get_module_name(module_t module) {
            std::string path = "/proc/self/stat";
            std::ifstream file(path);
            std::stringstream content;
            std::string line;
            while (std::getline(file, line)) {
                content << line << '\n';
            }
            file.close();
            std::string text = content.str();
            size_t begin = text.find_first_of('(');
            size_t end = text.find_first_of(')');
            size_t count = begin - (end + 1);
            return text.substr(begin, count);
        }
        std::string get_command_line() {
            FILE* f = fopen("/proc/self/cmdline", "rb");
            fseek(f, 0, SEEK_END);
            size_t length = ftell(f);
            rewind(f);
            char* block = (char*)malloc(length * sizeof(char));
            fread(block, sizeof(char), length, f);
            fclose(f);
            std::string text = std::string(block, length);
            free(block);
            char to_find = '\0';
            size_t pos = text.find(to_find);
            do {
                text.replace(pos, 1, " ");
                pos = text.find(to_find);
            } while (pos != std::string::npos);
            return text;
        }
        size_t get_last_os_error() {
            return 0;
        }
        gl_context_t make_opengl_context(window_t window, int major_version, int minor_version) {
            gl_context_t result;
            result.context = NULL;
            result.window = glx_functions._glXCreateWindow(display, fb_config, window, NULL);
            if (!result.window) {
                return result;
            }
            GLXContext temp = glx_functions._glXCreateNewContext(display, fb_config, GLX_RGBA_TYPE, NULL, true);
            if (!temp) {
                return result;
            }
            glx_functions._glXMakeContextCurrent(display, result.window, result.window, temp);
            unsigned int glew_error = glewInit();
            if (glew_error != GLEW_OK) {
                return result;
            }
            if (glxewIsSupported("GLX_ARB_create_context")) {
                int flags = 0;
#ifdef BANANA_DEBUG
                flags |= GLX_CONTEXT_DEBUG_BIT_ARB;
#endif
                int attribs[] = {
                    GLX_CONTEXT_MAJOR_VERSION_ARB, major_version,
                    GLX_CONTEXT_MINOR_VERSION_ARB, minor_version,
                    GLX_CONTEXT_FLAGS_ARB, flags,
                    None
                };
                result.context = glXCreateContextAttribsARB(display, fb_config, NULL, true, attribs);
                if (!result.context) {
                    return result;
                }
                glXDestroyContext(display, temp);
                glx_functions._glXMakeContextCurrent(display, result.window, result.window, result.context);
            } else {
                result.context = temp;
            }
            return result;
        }
        gl_context_t make_null_context(const gl_context_t& current) {
            return { NULL, current.window };
        }
        void destroy_context(const gl_context_t& context) {
            glXDestroyContext(display, context.context);
        }
        void swap_buffers(const gl_context_t& context) {
            glXSwapBuffers(display, context.window);
        }
        void bind(const gl_context_t& context) {
            glx_functions._glXMakeContextCurrent(display, context.window, context.window, context.context);
        }
        void init_imgui(window_t window) {
            ImGui_ImplX11_Init(window);
            size_t width, height;
            get_size(window, width, height);
            ImGuiPlatformIO& io = ImGui::GetPlatformIO();
            for (int i = 0; i < io.Monitors.size(); i++) {
                io.Monitors[i].WorkSize = ImVec2(width, height);
            }
        }
        void newframe_imgui() {
            ImGui_ImplX11_NewFrame();
        }
        void shutdown_imgui() {
            ImGui_ImplX11_Shutdown();
        }
    }
}