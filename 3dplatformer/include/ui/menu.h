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
				std::string text;
				std::vector<node> children;
				node_type type;
			};
			menu();
			menu(const std::string& json_file);
			~menu();
			void load_from_json_file(const std::string& json_file);
			void draw(SkCanvas* canvas);
			void draw_node(SkCanvas* canvas, node& n);
			void set_ptrs(game* g_game, lua_interpreter* interpreter);
		private:
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