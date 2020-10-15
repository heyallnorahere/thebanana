#pragma once
namespace thebanana {
	class game;
	namespace ui {
		class menu {
		public:
			enum class node_type {
				text,
				rectangle,
				ellipse,
				other,
			};
			struct node {
				float x, y, w, h;
				glm::vec4 color;
				std::string text;
				std::vector<node> children;
				node_type type;
			};
			menu();
			menu(const std::string& json_file);
			void load_from_json_file(const std::string& json_file);
			void draw(SkCanvas* canvas);
			void draw_node(SkCanvas* canvas, node& n);
			void set_game_ptr(game* g_game);
		private:
			std::vector<node> children;
			SkPaint paint;
			SkFont font;
			game* g_game;
			glm::vec4 clear_color;
		};
	}
}