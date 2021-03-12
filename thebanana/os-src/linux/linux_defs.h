#pragma once
namespace thebanana {
    namespace platform_specific {
        using window_t = xcb_window_t;
        using cursor_t = size_t;
        using module_t = size_t;
        struct gl_context_t {
            GLXContext context;
            GLXWindow window;
        };
        enum class cursor_types {
            none,
            arrow,
        };
    }
}