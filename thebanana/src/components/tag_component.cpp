#include "pch.h"
#include "components/tag_component.h"
namespace thebanana {
	tag_component::tag_component(gameobject* object) : component(object) {
		this->add_property(new property<std::string>("", "Value"));
	}
	void tag_component::set_tag(const std::string& tag) {
		this->set_property<std::string>("Value", tag);
	}
	std::string tag_component::get_tag() {
		return *this->get_property<std::string>("Value");
	}
}