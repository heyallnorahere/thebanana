#include "pch.h"
#include "components/light_component.h"
namespace thebanana {
	light_component::light_component(gameobject* object) : component(object) {
		this->add_property(new property<property_classes::color>(glm::vec3(1.f), "Diffuse"));
		this->add_property(new property<property_classes::color>(glm::vec3(1.f), "Specular"));
		this->add_property(new property<property_classes::color>(glm::vec3(1.f), "Ambient"));
		this->add_property(new property<float>(0.1f, "Ambient strength"));
	}
	light_component::light_data light_component::get_data() {
		light_data data;
		data.position = this->get_transform();
		data.diffuse = this->get_property<property_classes::color>("Diffuse")->get_vector();
		data.specular = this->get_property<property_classes::color>("Specular")->get_vector();
		data.ambient = this->get_property<property_classes::color>("Ambient")->get_vector();
		data.ambient_strength = *this->get_property<float>("Ambient strength");
		return data;
	}
}