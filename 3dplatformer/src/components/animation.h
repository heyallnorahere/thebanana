#pragma once
#include "component.h"
namespace thebanana {
	class gameobject;
	class animation_component : public component {
	public:
		animation_component(gameobject* obj);
		virtual void post_update() override;
		void start_animation(int index, bool repeat = false);
		void start_animation(const std::string& name, bool repeat = false);
		void stop_animation();
		double get_animation_time();
		int get_animation_id() const;
		bool is_animating() const;
	private:
		int animation_id;
		double animation_start_time;
		bool repeat;
	};
}