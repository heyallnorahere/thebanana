#include "pch.h"
#include "prop.h"
#include "debug_tools.h"
#include "rigidbody.h"
prop::prop(const std::string& model_name) : m_model_name(model_name) {
	this->m_nickname = "prop";
	this->add_property(new component::property<std::string>(model_name, "model name"));
	this->add_component<rigidbody>();
}
void prop::update() {
	this->prepare_for_update();
	this->post_update();
}
void prop::render() {
	this->prepare_for_render();
	component::property<std::string>* model_name = this->find_property<std::string>("model name");
	this->render_model(model_name ? *model_name->get_value() : this->m_model_name);
	this->post_render();
}
prop::~prop() { }
std::string prop::get_model_name() {
	component::property<std::string>* model_name = this->find_property<std::string>("model name");
	return model_name ? *model_name->get_value() : this->m_model_name;
}
