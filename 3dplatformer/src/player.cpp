#include "pch.h"
#include "player.h"
#include "game.h"
#include "input_manager.h"
#include "mouse.h"
#include "controller.h"
#include "scene.h"
#include "debug_tools.h"
#include "prop.h"
player::player() {
	log_print("created player\n");
	this->add_component<animation_component>();
	this->m_nickname = "player";
	this->m_walking = false;
}
void player::update() {
	if (this->m_game->get_current_frame() == 1 && this->get_number_components<animation_component>() > 0) {
		this->get_component<animation_component>().start_animation("idle", true);
	}
	const float speed = 0.05f;
	this->prepare_for_update();
#ifdef _DEBUG
	if (control) {
#endif
	glm::vec3 translate(0.f);
	if (this->m_game->get_input_manager()->get_device_type(0) == input_manager::device_type::keyboard) {
		std::vector<input_manager::device::button> btns = this->m_game->get_input_manager()->get_device_buttons(0);
		if (btns[DIK_W].held) {
			translate.z += speed;
		}
		if (btns[DIK_S].held) {
			translate.z -= speed;
		}
		if (btns[DIK_A].held) {
			translate.x += speed;
		}
		if (btns[DIK_D].held) {
			translate.x -= speed;
		}
		if ((btns[DIK_W].down || btns[DIK_S].down || btns[DIK_A].down || btns[DIK_D].down) && !this->m_walking && this->get_number_components<animation_component>() > 0) {
			this->get_component<animation_component>().stop_animation();
			this->get_component<animation_component>().start_animation("walk", true);
			this->m_walking = true;
		}
		if (this->m_walking && !(btns[DIK_W].held || btns[DIK_S].held || btns[DIK_A].held || btns[DIK_D].held) && this->get_number_components<animation_component>() > 0) {
			this->get_component<animation_component>().stop_animation();
			this->get_component<animation_component>().start_animation("idle", true);
			this->m_walking = false;
		}
	}
	else if (this->m_game->get_input_manager()->get_device_type(0) == input_manager::device_type::controller) {
		glm::vec2 left_stick = ((controller*)this->m_game->get_input_manager()->get_device(0))->get_joysticks().left;
		translate += glm::vec3(-left_stick.x, 0.f, left_stick.y) * speed * 2.f;
	}
	this->m_transform.translate(translate);
#ifdef _DEBUG
	}
#endif
	this->post_update();
	log_print("updated player");
}
void player::render() {
	this->prepare_for_render();
	this->render_model("waluigi");
	this->post_render();
	log_print("rendered player");
}
player::~player() { }
std::string player::get_model_name() {
	return "waluigi";
}
