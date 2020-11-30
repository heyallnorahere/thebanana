#include "pch.h"
#include "gameobject.h"
#include "components/native_script_component.h"
namespace thebanana {
	script::script(gameobject* object, native_script_component* nsc) {
		this->parent = object;
		this->interface = nsc;
	}
	void script::initialize() { }
	void script::pre_update() { }
	void script::update() { }
	void script::post_update() { }
	void script::pre_render() { }
	void script::render() { }
	void script::post_render() { }
	void script::on_collision(gameobject* other) { }
	script::~script() { }
	void script::add_property(component::property_base* p) {
		this->interface->add_property(p);
		p->set_game(this->parent->get_game());
	}
	transform& script::get_transform() {
		return this->parent->get_transform();
	}
	native_script_component::native_script_component(gameobject* object) : component(object) {
		this->m_destroy_script = NULL;
		this->m_script = NULL;
		this->add_property(new property<property_base::read_only_text>(property_base::read_only_text("none"), "Script"));
	}
	void native_script_component::pre_update() {
		if (this->m_script) this->m_script->pre_update();
	}
	void native_script_component::update() {
		if (this->m_script) this->m_script->update();
	}
	void native_script_component::post_update() {
		if (this->m_script) this->m_script->post_update();
	}
	void native_script_component::pre_render() {
		if (this->m_script) this->m_script->pre_render();
	}
	void native_script_component::render() {
		if (this->m_script) this->m_script->render();
	}
	void native_script_component::post_render() {
		if (this->m_script) this->m_script->post_render();
	}
	void native_script_component::on_collision(gameobject* other) {
		if (this->m_script) this->m_script->on_collision(other);
	}
	void native_script_component::clean_up() {
		if (this->m_destroy_script) this->m_destroy_script(this);
	}
}