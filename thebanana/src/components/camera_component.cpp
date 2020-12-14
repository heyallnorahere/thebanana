#include "pch.h"
#include "components/camera_component.h"
#include "gameobject.h"
#include "game.h"
#include "scene.h"
namespace thebanana {
	camera_component::camera_component(gameobject* object) : component(object) {
		this->direction = glm::vec3(0.f, 0.f, 1.f);
		this->add_property(new property<glm::vec2>(glm::vec2(0.f, -90.f), "Angle"));
		this->add_property(new property<bool>(false, "Primary"));
	}
	void camera_component::update() {
		glm::vec2 angle = *this->get_property<glm::vec2>("Angle");
		glm::vec3 d;
		d.x = cos(glm::radians(angle.y)) * cos(glm::radians(angle.x));
		d.y = sin(glm::radians(angle.x));
		d.z = sin(glm::radians(angle.y)) * cos(glm::radians(angle.x));
		this->direction = glm::normalize(d);
	}
	void camera_component::render() {
		if (*this->get_property<bool>("Primary")) {
			this->parent->get_scene()->get_current_shader()->get_uniforms().mat4("projection", this->calculate_projection());
			this->parent->get_scene()->get_current_shader()->get_uniforms().mat4("view", this->calculate_view());
		}
	}
	glm::mat4 camera_component::calculate_projection() {
		glm::mat4 projection = glm::perspective(glm::radians(45.f), this->parent->get_game()->get_aspect_ratio(), 0.1f, 100.f);
		return projection;
	}
	glm::mat4 camera_component::calculate_view() {
		glm::vec3 pos = this->get_transform();
		glm::mat4 view = glm::lookAt(pos, pos + this->direction, glm::vec3(0.f, 1.f, 0.f));
		return view;
	}
}