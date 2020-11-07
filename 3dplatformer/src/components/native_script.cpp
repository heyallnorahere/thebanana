#include "pch.h"
#include "components/native_script.h"
#include "gameobject.h"
namespace thebanana {
	script::script(gameobject* object) : parent(object) {
		this->interface = &(this->parent->get_component<native_script_component>());
	}
	void script::pre_update() { }
	void script::update() { }
	void script::post_update() { }
	void script::pre_render() { }
	void script::render() { }
	void script::post_render() { }
	void script::on_collision(gameobject* other) { }
	void script::clean_up() { }
	native_script_component::native_script_component(gameobject* object) : component(object) {
		this->m_destroy_script = NULL;
		this->m_script = NULL;
		this->add_property(new property<property_base::read_only_text>(property_base::read_only_text("none"), "script"));
	}
	script* native_script_component::get_script() {
		return this->m_script;
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
		if (this->m_script) this->m_script->clean_up();
		if (this->m_destroy_script) this->m_destroy_script(this);
	}
}