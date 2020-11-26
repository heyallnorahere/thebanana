#pragma once
namespace thebanana {
	class game;
	namespace debug {
		void init_imgui(HWND window);
		void clean_up_imgui();
		void render_imgui(game* g_game);
		extern bool control;
	}
}