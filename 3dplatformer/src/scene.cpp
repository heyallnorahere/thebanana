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
	OutputDebugString(TEXT("updating\n"));
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
	this->update_player_angle();
	for (auto& c : this->m_children) {
		c->update();
	}
}
void scene::render() {
	OutputDebugString(TEXT("rendering\n"));
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
void scene::update_player_angle() {
	static glm::mat4 last_inverse(1.f);
	static glm::vec2 offset(0.f);
	glm::vec2 result;
	if (this->m_game->get_input_manager()->get_device_type(0) == input_manager::device_type::controller) {
		controller* c = (controller*)this->m_game->get_input_manager()->get_device(0);
		result = c->get_joysticks().right;
	} else {
		static glm::vec2 last(0.f);
		mouse* m = (mouse*)this->m_game->get_input_manager()->get_device(1);
		glm::vec2 current = m->get_pos();
		result = current - last;
		last = current;
	}
	result *= glm::vec2(-10.f);
	offset += result;
	const float limit = 89.f;
	if (offset.y > limit)
		offset.y = limit;
	if (offset.y < -limit)
		offset.y = -limit;
	transform t;
	t.rotate(offset.x, glm::vec3(0.f, 1.f, 0.f));
	t.rotate(offset.y, glm::vec3(1.f, 0.f, 0.f));
	this->m_player->get_transform() *= last_inverse;
	this->m_player->get_transform() *= t;
	last_inverse = glm::inverse(t.get_matrix());
}
