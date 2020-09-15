#include "pch.h"
#include "prop.h"
#include "debug_tools.h"
#include "rigidbody.h"
prop::prop(const std::string& model_name) {
	this->m_nickname = "prop";
	this->add_property(new component::property<std::string>(model_name, "model name"));
	this->add_component<mesh_component>().set_mesh_name(model_name);
}
void prop::update() {
	this->prepare_for_update();
	if (this->get_number_components<mesh_component>() > 0) {
		component::property<std::string>* mn = this->find_property<std::string>("model name");
		if (mn) {
			if (this->get_component<mesh_component>().get_mesh_name() != *mn->get_value()) {
				this->get_component<mesh_component>().set_mesh_name(*mn->get_value());
			}
		}
	}
	this->post_update();
}
void prop::render() {
	this->prepare_for_render();
	component::property<std::string>* model_name = this->find_property<std::string>("model name");
	if (this->get_number_components<mesh_component>() > 0) {
		this->get_component<mesh_component>().render();
	}
	this->post_render();
}
prop::~prop() { }