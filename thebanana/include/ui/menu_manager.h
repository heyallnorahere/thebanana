#pragma once
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
			menu_manager(game* g_game);
			void load_menu(menu* m);
			void update();
			void draw();
			graphics::texture* get_texture();
			~menu_manager();
			void update_texture_pixels();
			void open_menus();
			void close_menus();
			void toggle_menus();
			bool menus_open();
		private:
			void setup_canvas();
			void setup_texture();
			void destroy_texture();
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
