#include "pch.h"
#include "scene.h"
#include "game.h"
#include "player.h"
#include "camera.h"
#include "input_manager.h"
#include "controller.h"
#include "mouse.h"
#include "keyboard.h"
#include "../resource.h"
#include "debug_tools.h"
scene::scene(game* g) {
	this->m_shader = std::unique_ptr<opengl_shader_library::shader>(new opengl_shader_library::win32_resource_shader(IDR_BASIC_VERTEX, IDR_BASIC_FRAGMENT, "SHADER"));
	this->m_game = g;
	this->m_player = new player;
	this->m_camera = new camera(this->m_player);
	this->add_object(this->m_camera);
	this->add_object(this->m_player);
}
void scene::add_object(gameobject* obj) {
	obj->init(ROOT, this, this->m_game);
	this->m_children.push_back(std::unique_ptr<gameobject>(obj));
}
void scene::update() {
	log_print("updating");
#ifdef _DEBUG
	{
		bool toggle = false;
		if (this->m_game->get_input_manager()->get_device_type(0) == input_manager::device_type::keyboard) {
			keyboard* kb = (keyboard*)this->m_game->get_input_manager()->get_device(0);
			std::vector<keyboard::button> btns = kb->get_buttons();
			toggle = btns[DIK_F1].down;
		}
		if (toggle) {
			control = !control;
		}
	}
	if (control)
#endif
	this->update_camera_angle();
	for (auto& c : this->m_children) {
		c->update();
	}
}
void scene::render() {
	log_print("rendering");
	opengl_shader_library::shader::use(this->m_shader.get());
	glm::mat4 projection = glm::perspective(glm::radians(45.f), this->m_game->get_aspect_ratio(), 0.1f, 100.f);
	this->m_shader->get_uniforms().mat4("projection", projection);
	for (auto& c : this->m_children) {
		c->render();
	}
	opengl_shader_library::shader::use(NULL);
}
size_t scene::get_children_count() const {
	return this->m_children.size();
}
const gameobject* scene::get_child(size_t index) const {
	auto it = this->m_children.begin();
	std::advance(it, index);
	const auto& c = *it;
	return c.get();
}
gameobject* scene::get_child(size_t index) {
	auto it = this->m_children.begin();
	std::advance(it, index);
	const auto& c = *it;
	return c.get();
}
opengl_shader_library::shader* scene::get_shader() const {
	return this->m_shader.get();
}
player* scene::get_player() {
	return this->m_player;
}
camera* scene::get_camera() {
	return this->m_camera;
}
void scene::update_camera_angle() {
	
}
