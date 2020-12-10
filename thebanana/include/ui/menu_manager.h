#pragma once
#include "banana_api.h"
namespace thebanana {
	class game;
	class lua_interpreter;
	namespace graphics {
		class texture;
	}
	namespace ui {
		class menu;
		class menu_manager {
		public:
			BANANA_API menu_manager(game* g_game);
			BANANA_API void load_menu(menu* m);
			BANANA_API void update();
			BANANA_API void draw();
			BANANA_API graphics::texture* get_texture();
			BANANA_API ~menu_manager();
			BANANA_API void update_texture_pixels();
			BANANA_API void open_menus();
			BANANA_API void close_menus();
			BANANA_API void toggle_menus();
			BANANA_API bool menus_open();
		private:
			BANANA_API void setup_canvas();
			BANANA_API void setup_texture();
			BANANA_API void destroy_texture();
			graphics::texture* m_texture;
			game* m_game;
			menu* m_current_menu;
#ifdef BANANA_BUILD
			SkCanvas* m_canvas;
			sk_sp<SkSurface> m_surface;
#endif
			bool m_draw_menus;
			menu* m_last_frame_menu_ptr;
		};
	}
}
