#pragma once
namespace thebanana {
	class application_layer {
	public:
		virtual void init();
		virtual void gameloop();
		virtual void clean_up();
		virtual std::string window_title();
	};
}