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
			void draw();
			graphics::texture* get_texture();
			~menu_manager();
			void update_canvas_size();
			void update_texture_pixels();
			void open_menus();
			void close_menus();
			void toggle_menus();
			bool menus_open();
		private:
			void setup_canvas();
			void setup_texture();
			void destroy_texture();
			sk_sp<SkImage> get_canvas_data();
			graphics::texture* m_texture;
			game* m_game;
			menu* m_current_menu;
			lua_interpreter* m_interpreter;
			SkCanvas* m_canvas;
			sk_sp<SkSurface> m_surface;
			bool m_draw_menus;
		};
	}
}
