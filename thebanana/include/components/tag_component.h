#pragma once
#include "component.h"
namespace thebanana {
	class gameobject;
	class tag_component : public component {
	public:
		tag_component(gameobject* object);
		void set_tag(const std::string& tag);
		std::string get_tag();
	};
}