#pragma once
#include "util.h"
#include "model_registry.h"
#define BANANA_WINDOW_CLASS_NAME "banana_ui"
namespace thebanana {
	class scene;
	class input_manager;
	class framebuffer;
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
		static long long __stdcall wndproc(HWND window, unsigned int msg, unsigned long long w_param, long long l_param);
		float get_aspect_ratio();
		scene* get_scene();
		void add_model_desc(const model_registry::model_descriptor& desc);
		void load_models();
		HWND get_window();
	private:
		unsigned int m_frame;
		HWND m_window;
		opengl_viewport* m_viewport;
		scene* m_scene;
		input_manager* m_input_manager;
		model_registry* m_model_registry;
		float m_aspect_ratio;
		std::vector<model_registry::model_descriptor> descriptors;
	};
	extern game* g_game;
}