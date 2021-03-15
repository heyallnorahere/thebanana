#include "pch.h"
#include "platform_misc.h"
#import <Cocoa/Cocoa.h>
#import <backends/imgui_impl_osx.h>
#include "macosx_internal.h"
#import "view.h"
#include <crt_externs.h>
namespace thebanana {
    namespace platform_specific {
        std::string get_module_name(module_t module) {
            char** executable_name = _NSGetProgname();
            if (executable_name) {
                return *executable_name;
            } else {
                return "thebanana";
            }
        }
        std::string get_command_line() {
            return "";
        }
        size_t get_last_os_error() {
            return 0; // after writing a big chunk of this, i realized that windows doesnt really conform to standards
        }
        extern NSWindow* get_window(window_t window);
        gl_context_t make_opengl_context(window_t window, int major_version, int minor_version) {
#define MAX_ATTRIB_COUNT 40
            NSOpenGLPixelFormatAttribute attribs[MAX_ATTRIB_COUNT];
            int current_index = 0;
            auto add_attrib = [&](NSOpenGLPixelFormatAttribute attrib) {
                assert(current_index < MAX_ATTRIB_COUNT - 1);
                attribs[current_index++] = attrib;
            };
#define set_attrib(k, v) add_attrib(k); add_attrib(v)
            if (major_version >= 4) {
                set_attrib(NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core);
            }
            else if (major_version == 3) {
                set_attrib(NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core);
            }
            if (major_version > 2) {
                
            }
            constexpr int red = 8, green = 8, blue = 8, alpha = 8;
            constexpr int depth = 24;
            constexpr int stencil = 8;
            set_attrib(NSOpenGLPFAColorSize, red + green + blue);
            set_attrib(NSOpenGLPFAAlphaSize, alpha);
            set_attrib(NSOpenGLPFADepthSize, depth);
            set_attrib(NSOpenGLPFAStencilSize, stencil);
            add_attrib(NSOpenGLPFADoubleBuffer);
            add_attrib(0);
#undef MAX_ATTRIB_COUNT
#undef set_attrib
            NSOpenGLPixelFormat* format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
            assert(format != nil);
            gl_context_t context;
            NSWindow* _window = get_window(window);
            BananaContentView* view = (BananaContentView*)[_window contentView];
            context.view = view;
            context.context = [[NSOpenGLContext alloc] initWithFormat:format shareContext:nil];
            [(NSOpenGLContext*)context.context setView:view];
            assert(context.context != NULL);
            [format release];
            bind(context);
            return context;
        }
        gl_context_t make_null_context(const gl_context_t& current) {
            return { NULL, current.view };
        }
        void destroy_context(const gl_context_t& context) {
            [(NSOpenGLContext*)context.context release];
        }
        void swap_buffers(const gl_context_t& context) {
            [(NSOpenGLContext*)context.context flushBuffer];
        }
        void bind(const gl_context_t& context) {
            if (context.context) {
                [(NSOpenGLContext*)context.context makeCurrentContext];
            } else {
                [NSOpenGLContext clearCurrentContext];
            }
        }
        NSView* imgui_view = nil;
        void init_imgui(window_t window) {
            ImGui_ImplOSX_Init();
            NSWindow* _window = get_window(window);
            imgui_view = [_window contentView];
        }
        void newframe_imgui() {
            ImGui_ImplOSX_NewFrame(imgui_view);
        }
        void shutdown_imgui() {
            ImGui_ImplOSX_Shutdown();
        }
    }
}
