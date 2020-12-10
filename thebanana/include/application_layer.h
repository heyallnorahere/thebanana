#pragma once
#include "banana_api.h"
namespace thebanana {
	class BANANA_API application_layer {
	public:
		virtual void register_scripts();
		virtual void init();
		virtual void gameloop();
		virtual void clean_up();
		virtual std::string window_title();
	};
}