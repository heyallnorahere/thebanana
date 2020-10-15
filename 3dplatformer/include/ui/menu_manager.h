#pragma once
namespace thebanana {
	class game;
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
		private:
			void setup_canvas();
			void setup_texture();
			void destroy_texture();
			sk_sp<SkData> get_canvas_data();
			void update_texture_pixels();
			graphics::texture* m_texture;
			game* m_game;
			menu* m_current_menu;
			SkCanvas* m_canvas;
			sk_sp<SkSurface> m_surface;
		};
	}
}
