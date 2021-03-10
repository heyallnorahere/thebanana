#pragma once
#include "util.h"
#include "model_registry.h"
#include "script_module.h"
#include "banana_api.h"
#ifdef BANANA_BUILD
#include "window.h"
#endif
#define BANANA_WINDOW_CLASS_NAME "banana_ui"
namespace thebanana {
	class scene;
	class input_manager;
	class gameobject;
	class lua_interpreter;
	class shader_registry;
	class script_registry;
	class material_registry;
	class rigidbody;
	namespace ui {
		class menu_manager;
	}
	namespace sound {
		class sound_manager;
	}
#ifndef BANANA_BUILD
	namespace platform_specific {
		using window_t = size_t;
	}
#endif
	struct window {
		platform_specific::window_t m;
	};
	class game {
	public:
		BANANA_API game(const std::string& title, script_module::module_t module);
		BANANA_API ~game();
		BANANA_API void destroy();
		BANANA_API void update();
		BANANA_API void render();
		BANANA_API unsigned int get_current_frame();
		BANANA_API input_manager* get_input_manager();
		BANANA_API model_registry* get_model_registry();
		BANANA_API ui::menu_manager* get_menu_manager();
#ifdef BANANA_WINDOWS
		BANANA_API static long long __stdcall wndproc(HWND window, unsigned int msg, unsigned long long w_param, long long l_param);
#endif
		BANANA_API float get_aspect_ratio();
		BANANA_API scene* get_scene();
		BANANA_API void add_model_desc(const model_registry::model_descriptor& desc);
		BANANA_API void load_models();
		BANANA_API window& get_window();
		BANANA_API sound::sound_manager* get_sound_manager();
		BANANA_API lua_interpreter* get_lua_interpreter();
		BANANA_API bool& showing_cursor();
		BANANA_API void show_cursor();
		BANANA_API void hide_cursor();
		BANANA_API void toggle_cursor();
		BANANA_API std::string get_steam_name();
		BANANA_API shader_registry* get_shader_registry();
		BANANA_API script_registry* get_script_registry();
		BANANA_API material_registry* get_material_registry();
		BANANA_API void init_debug_menus();
		BANANA_API void init_steam();
		BANANA_API bool should_clip_cursor();
		BANANA_API void clip_cursor();
		BANANA_API void unclip_cursor();
		BANANA_API void toggle_cursor_clip();
		BANANA_API void make_context_current();
		BANANA_API void swap_buffers();
		BANANA_API void clear_screen();
		BANANA_API float get_timestep();
		BANANA_API std::vector<std::string> get_command_line();
		BANANA_API void load_script_module(const std::string& dllpath);
		BANANA_API void unload_script_module();
		BANANA_API void debug_print(const std::string& message);
		BANANA_API std::list<rigidbody*>& get_rigidbody_list();
		BANANA_API std::string get_debug_log();
		BANANA_API glm::vec2 get_window_size();
		BANANA_API void update_aspect_ratio();
		BANANA_API void set_aspect_ratio(float ratio);
		// very loose template stuff, but if you know what to do with it, it works
		template<typename T> using imgui_ptr = void(*)(const char*, T*);
		template<typename T> imgui_ptr<T> get_imgui_pointer();
	private:
		void shutdown_steam();
		void fill_imgui_input_pointers();
		std::stringstream m_debug_log;
		std::ofstream m_file_log;
		unsigned int m_frame;
		window m_window;
#ifndef BANANA_BUILD
		using opengl_viewport = void;
#endif
		opengl_viewport* m_viewport;
		scene* m_scene;
		input_manager* m_input_manager;
		model_registry* m_model_registry;
		shader_registry* m_shader_registry;
		script_registry* m_script_registry;
		material_registry* m_material_registry;
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
		// so you can call imgui across modules (dlls, exes, etc.)
		std::map<size_t, void*> m_imgui_input_functions;
		std::list<rigidbody*> m_rigidbodies;
		script_module::module_t m_executable;
	};
	BANANA_API extern game* g_game;
	template<typename T> inline game::imgui_ptr<T> game::get_imgui_pointer() {
		return (imgui_ptr<T>)this->m_imgui_input_functions[typeid(T).hash_code()];
	}
}