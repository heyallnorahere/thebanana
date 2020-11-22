#pragma once
namespace thebanana {
	class game;
	class lua_interpreter;
	namespace graphics {
		class texture;
	}
	namespace ui {
		class menu {
		public:
			enum class node_type {
				text,
				rectangle,
				ellipse,
				other,
			};
			enum class color_type {
				solid_color,
				texture,
			};
			template<color_type t> struct color_storage {
				int unused;
			};
			template<> struct color_storage<color_type::solid_color> {
				glm::vec4 c;
			};
#ifdef BANANA_BUILD
			template<> struct color_storage<color_type::texture> {
				sk_sp<SkImage> tex;
			};
#endif
			struct color {
				virtual color_type get_type() = 0;
				virtual void init(void* color_node) = 0;
			protected:
#ifdef BANANA_BUILD
				static sk_sp<SkImage> init_texture(void* color_node);
#endif
				static glm::vec4 init_color(void* color_node);
			};
			template<color_type t> struct color_impl : public color {
				virtual color_type get_type() override;
				virtual void init(void* color_node) override;
				color_storage<t> storage;
			};
			struct node {
				float x, y, w, h;
				color* c;
				std::string text, onclick;
				std::vector<node> children;
				node_type type;
				void get_top_node_at_pos(node*& ptr, glm::vec2 cursor_pos);
				void on_unload();
			};
			menu(const std::string& json_file);
			~menu();
			void load_from_json_file(const std::string& json_file);
			void update();
#ifdef BANANA_BUILD
			void draw(SkCanvas* canvas);
			void draw_node(SkCanvas* canvas, node& n);
#endif
			void set_ptrs(game* g_game);
			bool script_loaded();
		private:
			void on_click();
			menu();
			std::vector<node> children;
#ifdef BANANA_BUILD
			SkPaint paint;
			SkFont font;
#else
			int64_t unused[12]; // so the linker doesnt freak out that its a different size
#endif
			game* g_game;
			lua_interpreter* interpreter;
			glm::vec4 clear_color;
			bool has_script;
			std::string script_path;
		};
		template<menu::color_type t> inline menu::color_type menu::color_impl<t>::get_type() {
			return t;
		}
		inline void menu::color_impl<menu::color_type::solid_color>::init(void* color_node) {
			this->storage.c = init_color(color_node);
		}
#ifdef BANANA_BUILD
		inline void menu::color_impl<menu::color_type::texture>::init(void* color_node) {
			this->storage.tex = init_texture(color_node);
		}
#endif
	}
}