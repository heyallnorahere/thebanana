#pragma once
#include "util.h"
#include "model_registry.h"
#define BANANA_WINDOW_CLASS_NAME "banana_ui"
namespace thebanana {
	class scene;
	class input_manager;
	class gameobject;
	class lua_interpreter;
	class shader_registry;
	class script_registry;
	class script_module;
	namespace ui {
		class menu_manager;
	}
	namespace sound {
		class sound_manager;
	}
	class game {
	public:
		game(const std::string& title);
		~game();
		void destroy();
		void update();
		void render();
		unsigned int get_current_frame();
		input_manager* get_input_manager();
		model_registry* get_model_registry();
		ui::menu_manager* get_menu_manager();
		static long long __stdcall wndproc(HWND window, unsigned int msg, unsigned long long w_param, long long l_param);
		float get_aspect_ratio();
		scene* get_scene();
		void add_model_desc(const model_registry::model_descriptor& desc);
		void load_models();
		HWND get_window();
		sound::sound_manager* get_sound_manager();
		lua_interpreter* get_lua_interpreter();
		bool& showing_cursor();
		void show_cursor();
		void hide_cursor();
		void toggle_cursor();
		std::string get_steam_name();
		shader_registry* get_shader_registry();
		script_registry* get_script_registry();
		void init_debug_menus();
		void init_steam();
		bool should_clip_cursor();
		void clip_cursor();
		void unclip_cursor();
		void toggle_cursor_clip();
		void make_context_current();
		void swap_buffers();
		void clear_screen();
		float get_timestep();
		std::vector<std::string> get_command_line();
		void load_script_module(const std::string& dllpath);
		void debug_print(const std::string& message);
		std::string get_debug_log();
	private:
		void shutdown_steam();
		std::stringstream debug_log;
		unsigned int m_frame;
		HWND m_window;
#ifndef BANANA_BUILD
		using opengl_viewport = void;
#endif
		opengl_viewport* m_viewport;
		scene* m_scene;
		input_manager* m_input_manager;
		model_registry* m_model_registry;
		shader_registry* m_shader_registry;
		script_registry* m_script_registry;
		ui::menu_manager* m_menu_manager;
		sound::sound_manager* m_sound_manager;
		gameobject* m_menu_quad;
		lua_interpreter* m_interpreter;
		float m_aspect_ratio;
		std::vector<model_registry::model_descriptor> m_descriptors;
		bool m_show_cursor;
		bool m_debug_menus_initialized;
		bool m_steam_initialized;
		bool m_clip_cursor;
		script_module* m_module;
	};
	extern game* g_game;
}