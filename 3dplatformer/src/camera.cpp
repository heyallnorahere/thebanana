#include "pch.h"
#include "camera.h"
#include "player.h"
#include "scene.h"
#include "debug_tools.h"
#include "game.h"
#include "input_manager.h"
#include "controller.h"
#include "mouse.h"
namespace thebanana {
	camera::camera(player* p) : m_player(p) {
		debug::log_print("created camera");
		this->m_direction = glm::vec3(0.f, 0.f, 1.f);
		this->m_angle = glm::vec2(0.f, -90.f);
		this->m_nickname = "camera";
		this->add_property(new component::property<float>(2.f, "distance"));
	}
	void camera::update() {
		if (this->get_number_components<debug_component>() > 0) {
			this->remove_component<debug_component>();
		}
		this->prepare_for_update();
#ifdef _DEBUG
		if (debug::control) {
#endif
			glm::vec2 result;
			if (this->m_game->get_input_manager()->get_device_type(0) == input_manager::device_type::controller) {
				controller* c = (controller*)this->m_game->get_input_manager()->get_device(0);
				result = c->get_joysticks().right;
			}
			else {
				static glm::vec2 last(0.f);
				mouse* m = (mouse*)this->m_game->get_input_manager()->get_device(1);
				glm::vec2 current = m->get_pos();
				result = current - last;
				last = current;
			}
			result *= glm::vec2(10.f, -10.f);
			result = glm::vec2(result.y, result.x);
			this->m_angle += result;
			const float limit = 89.f;
			if (this->m_angle.x > limit)
				this->m_angle.x = limit;
			if (this->m_angle.x < -limit)
				this->m_angle.x = -limit;
			glm::vec3 d;
			d.x = cos(glm::radians(this->m_angle.y)) * cos(glm::radians(this->m_angle.x));
			d.y = sin(glm::radians(this->m_angle.x));
			d.z = sin(glm::radians(this->m_angle.y)) * cos(glm::radians(this->m_angle.x));
			this->m_direction = glm::normalize(d);
#ifdef _DEBUG
		}
#endif
		component::property<float>* distance = this->find_property<float>("distance");
		this->m_transform = transform().translate(glm::vec3(this->m_player->get_transform()) + this->m_direction * (distance ? *distance->get_value() : 2.f));
		this->post_update();
	}
	void camera::render() {
		this->prepare_for_render();
		glm::vec3 pos = this->m_transform.get_matrix() * glm::vec4(0.f, 1.f, 0.f, 1.f);
		glm::vec3 player_pos = this->m_player->get_transform().get_matrix() * glm::vec4(0.f, 0.75f, 0.f, 1.f);
		glm::mat4 rotation = this->m_player->get_transform().get_matrix();
		rotation[3] = glm::vec4(0.f, 0.f, 0.f, rotation[3].w);
		glm::vec3 up = glm::vec3(rotation * glm::vec4(0.f, 1.f, 0.f, 1.f));
		glm::mat4 view = glm::lookAt(pos, player_pos, up);
		this->m_scene->get_shader()->get_uniforms().mat4("view", view);
		this->post_render();
	}
	glm::vec3 camera::get_direction() {
		return this->m_direction;
	}
	glm::vec2 camera::get_angle() {
		return this->m_angle;
	}
}