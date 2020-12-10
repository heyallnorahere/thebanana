#pragma once
#include "component.h"
#include "banana_api.h"
namespace thebanana {
	class gameobject;
	class tag_component : public component {
	public:
		BANANA_API tag_component(gameobject* object);
		BANANA_API void set_tag(const std::string& tag);
		BANANA_API std::string get_tag();
	};
}