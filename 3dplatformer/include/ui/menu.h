#pragma once
namespace thebanana {
	class game;
	class lua_interpreter;
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
				std::string text, onclick;
				std::vector<node> children;
				node_type type;
				void get_top_node_at_pos(node*& ptr, glm::vec2 cursor_pos);
			};
			menu(const std::string& json_file);
			~menu();
			void load_from_json_file(const std::string& json_file);
			void update();
			void draw(SkCanvas* canvas);
			void draw_node(SkCanvas* canvas, node& n);
			void set_ptrs(game* g_game, lua_interpreter* interpreter);
			bool script_loaded();
		private:
			void on_click();
			menu();
			std::vector<node> children;
			SkPaint paint;
			SkFont font;
			game* g_game;
			lua_interpreter* interpreter;
			glm::vec4 clear_color;
			bool has_script;
			std::string script_path;
		};
	}
}