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
	OutputDebugString(TEXT("created player\n"));
}
void player::update() {
	const float speed = 0.05f;
	this->prepare_for_update();
#ifdef _DEBUG
	if (control) {
#endif
	glm::vec3 translate(0.f);
	if (this->m_game->get_input_manager()->get_device_type(0) == input_manager::device_type::keyboard) {
		std::vector<input_manager::device::button> btns = this->m_game->get_input_manager()->get_device_buttons(0);
		if (btns[17].held) {
			translate.z += speed;
		}
		if (btns[31].held) {
			translate.z -= speed;
		}
		if (btns[30].held) {
			translate.x += speed;
		}
		if (btns[32].held) {
			translate.x -= speed;
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
	OutputDebugString(TEXT("updated player\n"));
}
void player::render() {
	this->prepare_for_render();
	this->render_model("waluigi");
	OutputDebugString(TEXT("rendered player\n"));
}
player::~player() { }
