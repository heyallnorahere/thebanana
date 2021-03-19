#pragma once
#include "banana_api.h"
namespace thebanana {
	class application_layer {
	public:
		BANANA_API virtual void register_scripts();
		BANANA_API virtual void init();
		BANANA_API void gameloop();
		BANANA_API virtual void update();
		BANANA_API virtual void render();
		BANANA_API virtual void clean_up();
		BANANA_API virtual std::string window_title();
	};
}