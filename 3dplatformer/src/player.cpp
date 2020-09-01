#include "pch.h"
#include "player.h"
#include "game.h"
#include "input_manager.h"
#include "mouse.h"
#include "controller.h"
#include "camera.h"
#include "scene.h"
#include "debug_tools.h"
#include "prop.h"
player::player() {
	log_print("created player\n");
	this->add_component<animation_component>();
	this->m_nickname = "player";
	this->m_walking = false;
	this->m_last_angle = glm::vec2(0.f, -90.f);
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
	glm::vec2 angle = this->m_scene->get_camera()->get_angle() + glm::vec2(0.f, 90.f);
	glm::vec3 r;
	r.x = cos(glm::radians(angle.y)) * cos(glm::radians(angle.x));
	r.y = sin(glm::radians(angle.x));
	r.z = sin(glm::radians(angle.y)) * cos(glm::radians(angle.x));
	glm::vec3 right = glm::normalize(r);
	if (this->m_game->get_input_manager()->get_device_type(0) == input_manager::device_type::keyboard) {
		std::vector<input_manager::device::button> btns = this->m_game->get_input_manager()->get_device_buttons(0);
		if (btns[DIK_W].held) {
			this->move(0.f, translate, speed);
		}
		if (btns[DIK_S].held) {
			//translate += this->m_scene->get_camera()->get_direction() * glm::vec3(1.f, 0.f, 1.f) * speed;
			this->move(180.f, translate, speed);
		}
		if (btns[DIK_A].held) {
			//translate += right * glm::vec3(1.f, 0.f, 1.f) * speed;
			this->move(-90.f, translate, speed);
		}
		if (btns[DIK_D].held) {
			//translate -= right * glm::vec3(1.f, 0.f, 1.f) * speed;
			this->move(90.f, translate, speed);
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
void player::move(float yaw_offset, glm::vec3& translate, const float speed) {
	glm::vec2 current_angle = this->m_last_angle - (this->m_scene->get_camera()->get_angle() + glm::vec2(0.f, yaw_offset));
	this->m_last_angle = this->m_scene->get_camera()->get_angle() + glm::vec2(0.f, yaw_offset);
	this->m_transform.rotate(current_angle.y, glm::vec3(0.f, 1.f, 0.f));
	translate.z += speed;
}
