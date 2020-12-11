#include "pch.h"
#include "components/light_component.h"
namespace thebanana {
	light_component::light_component(gameobject* object) : component(object) {
		this->add_property(new property<property_classes::color>(glm::vec3(1.f), "Color"));
	}
	light_component::light_data light_component::get_data() {
		light_data data;
		data.color = this->get_property<property_classes::color>("Color")->get_vector();
		return data;
	}
}