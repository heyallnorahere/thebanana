#pragma once
namespace thebanana {
    namespace platform_specific {
        struct window_data;
        struct _window_t {
            window_data* data;
            ~_window_t();
        };
        using window_t = size_t;
        using cursor_t = size_t;
        using module_t = size_t;
        struct gl_context_t {
            void* context;
            void* view;
        };
        enum class cursor_types {
            none,
            arrow
        };
    }
}
