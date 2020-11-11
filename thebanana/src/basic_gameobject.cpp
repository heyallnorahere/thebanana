#include "pch.h"
#include "basic_gameobject.h"
namespace thebanana {
	basic_gameobject::basic_gameobject() {
		this->m_nickname = "Gameobject";
	}
	void basic_gameobject::update() {
		this->prepare_for_update();
		this->update_components();
		this->post_update();
	}
	void basic_gameobject::render() {
		this->prepare_for_render();
		this->render_components();
		this->post_render();
	}
}