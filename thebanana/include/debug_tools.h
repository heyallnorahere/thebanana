#pragma once
#include "banana_api.h"
#include "game.h"
namespace thebanana {
	class game;
	namespace debug {
		BANANA_API void init_imgui(platform_specific::window_t window);
		BANANA_API void clean_up_imgui();
		BANANA_API void render_imgui(game* g_game);
		BANANA_API extern bool control;
	}
}