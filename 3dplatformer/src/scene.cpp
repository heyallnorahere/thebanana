#include "pch.h"
#include "scene.h"
#include "game.h"
#include "input_manager.h"
#include "controller.h"
#include "mouse.h"
#include "keyboard.h"
#include "debug_tools.h"
#include "shader_registry.h"
namespace thebanana {
	scene::scene(game* g) {
		this->m_shader = NULL;
		this->m_game = g;
	}
	void scene::clear() {
		this->m_children.clear();
	}
	void scene::update() {
#ifdef _DEBUG
		{
			bool toggle_control = false, toggle_cursor = false;
			if (this->m_game->get_input_manager()->get_device_type(0) == input_manager::device_type::keyboard) {
				keyboard* kb = (keyboard*)this->m_game->get_input_manager()->get_device(0);
				std::vector<keyboard::button> btns = kb->get_buttons();
				toggle_control = btns[DIK_F1].down;
				toggle_cursor = btns[DIK_F2].down;
			}
			if (toggle_control) {
				debug::control = !debug::control;
			}
			if (toggle_cursor) {
				this->m_game->toggle_cursor();
			}
		}
#endif
		for (auto& c : this->m_children) {
			c->update();
		}
	}
	void scene::render() {
		opengl_shader_library::shader::use(this->m_shader);
		for (auto& c : this->m_children) {
			opengl_shader_library::shader::use(this->m_shader);
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
		return this->m_shader;
	}
	void scene::set_shader_name(const std::string& shader_name) {
		this->m_shader = this->m_game->get_shader_registry()->get_shader(shader_name);
	}
	game* scene::get_game() {
		return this->m_game;
	}
}