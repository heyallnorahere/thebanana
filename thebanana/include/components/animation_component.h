#pragma once
#include "component.h"
#include "banana_api.h"
namespace thebanana {
	class gameobject;
	class animation_component : public component {
	public:
		BANANA_API animation_component(gameobject* obj);
		BANANA_API virtual void post_update() override;
		BANANA_API void start_animation(int index, bool repeat = false);
		BANANA_API void start_animation(const std::string& name, bool repeat = false);
		BANANA_API void stop_animation();
		BANANA_API double get_animation_time();
		BANANA_API int get_animation_id() const;
		BANANA_API bool is_animating() const;
	private:
		int animation_id;
		double animation_start_time;
		bool repeat;
	};
}