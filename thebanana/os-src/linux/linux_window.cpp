#include "pch.h"
#include "window.h"
#include "game.h"
#include "application_layer.h"
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <dlfcn.h>
#include "glx_functions.h"
glx_functions_t glx_functions;
glx_functions_t::~glx_functions_t() {
    dlclose(this->opengl_lib);
}
xcb_connection_t* connection;
xcb_screen_t* screen;
xcb_atom_t wm_delete_window;
Display* display;
GLXFBConfig fb_config;
int visual_id;
void load_glx() {
    // this is what glfw did
    glx_functions.opengl_lib = dlopen("libGL.so", RTLD_LAZY);
    glx_functions._glXChooseFBConfig = (glx_functions_t::CHOOSEFBCONFIG)dlsym(glx_functions.opengl_lib, "glXChooseFBConfig");
    glx_functions._glXGetFBConfigAttrib = (glx_functions_t::GETFBCONFIGATTRIB)dlsym(glx_functions.opengl_lib, "glXGetFBConfigAttrib");
    glx_functions._glXCreateWindow = (glx_functions_t::CREATEWINDOW)dlsym(glx_functions.opengl_lib, "glXCreateWindow");
    glx_functions._glXCreateNewContext = (glx_functions_t::CREATENEWCONTEXT)dlsym(glx_functions.opengl_lib, "glXCreateNewContext");
    glx_functions._glXMakeContextCurrent = (glx_functions_t::MAKECONTEXTCURRENT)dlsym(glx_functions.opengl_lib, "glXMakeContextCurrent");
}
namespace thebanana {
    namespace platform_specific {
        void setup_window() {
            display = XOpenDisplay(NULL);
            auto default_screen = DefaultScreen(display);
            connection = XGetXCBConnection(display);
            XSetEventQueueOwner(display, XCBOwnsEventQueue);
            auto setup = xcb_get_setup(connection);
            auto it = xcb_setup_roots_iterator(setup);
            for (int i = default_screen; it.rem && i > 0; i--, xcb_screen_next(&it));
            screen = it.data;
            wm_delete_window = xcb_intern_atom_reply(connection, xcb_intern_atom(connection, 1, 16, "WM_DELETE_WINDOW"), NULL)->atom;
            unsigned int visual_attribs[] = {
                GLX_X_RENDERABLE, True,
                GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                GLX_RENDER_TYPE, GLX_RGBA_BIT,
                GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
                GLX_RED_SIZE, 8,
                GLX_GREEN_SIZE, 8,
                GLX_BLUE_SIZE, 8,
                GLX_ALPHA_SIZE, 8,
                GLX_DEPTH_SIZE, 24,
                GLX_STENCIL_SIZE, 8,
                GLX_DOUBLEBUFFER, True,
                None
            };
            load_glx();
            int num_fb_configs = 0;
            GLXFBConfig* fb_configs = glx_functions._glXChooseFBConfig(display, default_screen, (int*)visual_attribs, &num_fb_configs);
            assert(fb_configs && num_fb_configs);
            fb_config = fb_configs[0];
            glx_functions._glXGetFBConfigAttrib(display, fb_config, GLX_VISUAL_ID, &visual_id);
        }
        int window_loop(application_layer* app_layer) {
            xcb_generic_event_t* event;
            bool running = true;
            while (running) {
                if (event = xcb_poll_for_event(connection)) {
                    ImGui_ImplX11_ProcessEvent(event);
                    switch (event->response_type & ~0x80) {
                    case XCB_RESIZE_REQUEST:
                        if (g_game) {
                            g_game->update_aspect_ratio();
                            size_t width, height;
                            get_size(g_game->get_window().m, width, height);
                            glViewport(0, 0, width, height);
                        }
                        break;
                    case XCB_EXPOSE:
                        xcb_flush(connection);
                        break;
                    case XCB_CLIENT_MESSAGE:
                    {
                        auto client_message = (xcb_client_message_event_t*)event;
                        if (client_message->data.data32[0] == wm_delete_window) {
                            running = false;
                        }
                    }
                        break;
                    default:
                        break;
                    }
                }
                app_layer->gameloop();
            }
            return 0;
        }
        window_t create_window(const std::string& title, size_t width, size_t height, game* g) {
            xcb_colormap_t colormap = xcb_generate_id(connection);
            xcb_create_colormap(connection, XCB_COLORMAP_ALLOC_NONE, colormap, screen->root, visual_id);
            uint32_t value_list[] = {
                XCB_EVENT_MASK_EXPOSURE,
                colormap,
                NULL,
            };
            xcb_window_t window = xcb_generate_id(connection);
            xcb_create_window(connection, 24, window, screen->root, 0, 0, width, height, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT, visual_id, XCB_CW_EVENT_MASK | XCB_CW_COLORMAP, value_list);
            xcb_atom_t wm_protocols = xcb_intern_atom_reply(connection, xcb_intern_atom(connection, 1, 12, "WM_PROTOCOLS"), NULL)->atom;
            xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, wm_protocols, XCB_ATOM_ATOM, 32, 1, &wm_delete_window);
            xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, title.length(), title.c_str());
            xcb_map_window(connection, window);
            xcb_flush(connection);
            return window;
        }
        size_t get_width(window_t window) {
            auto cookie = xcb_get_geometry(connection, window);
            auto reply = xcb_get_geometry_reply(connection, cookie, NULL);
            assert(reply);
            uint16_t width = reply->width;
            free(reply);
            return (size_t)width;
        }
        size_t get_height(window_t window) {
            auto cookie = xcb_get_geometry(connection, window);
            auto reply = xcb_get_geometry_reply(connection, cookie, NULL);
            assert(reply);
            uint16_t height = reply->height;
            free(reply);
            return (size_t)height;
        }
        void get_size(window_t window, size_t& width, size_t& height) {
            auto cookie = xcb_get_geometry(connection, window);
            auto reply = xcb_get_geometry_reply(connection, cookie, NULL);
            if (reply) {
                width = (size_t)reply->width;
                height = (size_t)reply->height;
            } else {
                width = height = 0;
            }
            free(reply);
        }
        void set_window_title(window_t window, const std::string& title) {
            xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, title.length(), title.c_str());
        }
        void destroy_window(window_t window) {
            xcb_destroy_window(connection, window);
        }
        window_t get_focus() {
            // todo: implement
            return NULL;
        }
    }
}