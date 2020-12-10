#pragma once
#include "banana_api.h"
namespace thebanana {
	class game;
	namespace debug {
		BANANA_API void init_imgui(HWND window);
		BANANA_API void clean_up_imgui();
		BANANA_API void render_imgui(game* g_game);
		BANANA_API extern bool control;
	}
}