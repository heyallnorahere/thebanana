#include <thebanana.h>
#include "player.h"
#include "camera.h"
const float speed = 0.01f;
player_behavior::player_behavior(thebanana::gameobject* object, thebanana::native_script_component* nsc) : script(object, nsc) {
	thebanana::debug::log_print("created player");
	this->add_property(new thebanana::component::property<float>(speed, "speed"));
	this->add_property(new thebanana::component::property<thebanana::gameobject*>(NULL, "camera"));
	this->add_property(new thebanana::component::property<glm::vec2>(glm::vec2(0.f, -90.f), "last_camera_angle"));
	this->m_walking = false;
}
void player_behavior::initialize() {
	this->add_component<thebanana::animation_component>();
	this->add_component<thebanana::mesh_component>().set_mesh_name("player");
	this->add_component<thebanana::rigidbody>().set_check_for_collisions(true).set_speed_cap(0.5f).set_collider_type<thebanana::mlfarrel_model>().set_radius(0.4f).set_origin_offset(glm::vec3(0.f, 0.6f, 0.f));
	this->get_component<thebanana::rigidbody>().set_property("mass", 2.5f);
	this->parent->add_tag("player");
	this->parent->get_nickname() = "player";
}
void player_behavior::update() {
	if (this->get_number_components<thebanana::animation_component>() > 0) {
		if (!this->get_component<thebanana::animation_component>().is_animating()) {
			this->get_component<thebanana::animation_component>().start_animation("idle", true);
		}
	}
#ifdef _DEBUG
	if (thebanana::debug::control) {
#endif
		glm::vec3 translate(0.f);
		std::vector<float> angles;
		if (this->parent->get_game()->get_input_manager()->get_device_type(0) == thebanana::input_manager::device_type::keyboard) {
			std::vector<thebanana::input_manager::device::button> btns = this->parent->get_game()->get_input_manager()->get_device_buttons(0);
			if (btns[DIK_W].held) {
				angles.push_back(0.f);
			}
			if (btns[DIK_A].held) {
				angles.push_back(-90.f);
			}
			if (btns[DIK_D].held) {
				angles.push_back(90.f);
			}
			if (btns[DIK_S].held) {
				angles.push_back(btns[DIK_A].held ? -180.f : 180.f);
			}
			if (btns[DIK_TAB].down) {
				this->parent->get_game()->get_menu_manager()->toggle_menus();
			}
			if ((btns[DIK_W].down || btns[DIK_S].down || btns[DIK_A].down || btns[DIK_D].down) && !this->m_walking && this->get_number_components<thebanana::animation_component>() > 0) {
				this->get_component<thebanana::animation_component>().stop_animation();
				this->get_component<thebanana::animation_component>().start_animation("walk", true);
				this->m_walking = true;
			}
			if (this->m_walking && !(btns[DIK_W].held || btns[DIK_S].held || btns[DIK_A].held || btns[DIK_D].held) && this->get_number_components<thebanana::animation_component>() > 0) {
				this->get_component<thebanana::animation_component>().stop_animation();
				this->get_component<thebanana::animation_component>().start_animation("idle", true);
				this->m_walking = false;
			}
		}
		else if (this->parent->get_game()->get_input_manager()->get_device_type(0) == thebanana::input_manager::device_type::controller) {
			constexpr float deadzone = 0.05f;
			glm::vec2 left_stick = ((thebanana::controller*)this->parent->get_game()->get_input_manager()->get_device(0))->get_joysticks().left;
			float angle = atan2(left_stick.x, left_stick.y) * (180.f / static_cast<float>(M_PI));
			if (fabs(left_stick.x) > deadzone && fabs(left_stick.y) > deadzone) {
				angles.push_back(angle);
				if (this->get_number_components<thebanana::animation_component>() > 0 && !this->m_walking) {
					this->get_component<thebanana::animation_component>().stop_animation();
					this->get_component<thebanana::animation_component>().start_animation("walk", true);
					this->m_walking = true;
				}
			} else {
				if (this->get_number_components<thebanana::animation_component>() > 0 && this->m_walking) {
					this->get_component<thebanana::animation_component>().stop_animation();
					this->get_component<thebanana::animation_component>().start_animation("idle", true);
					this->m_walking = false;
				}
			}
			std::vector<thebanana::input_manager::device::button> btns = this->parent->get_game()->get_input_manager()->get_device_buttons(0);
			if (btns[6].down) { // select/touchpad
				this->parent->get_game()->get_menu_manager()->toggle_menus();
			}
			if (btns[7].down) { // start/menu/options
				this->parent->get_game()->destroy();
			}
		}
		if (angles.size() > 0) {
			float angle = 0.f;
			for (float a : angles) {
				angle += a;
			}
			angle /= angles.size();
			thebanana::component::property<float>* _speed = this->find_property<float>("speed");
			this->move(angle, translate, _speed ? *_speed->get_value() : speed);
		}
		glm::mat4 rotation = this->get_transform();
		rotation[3] = glm::vec4(0.f, 0.f, 0.f, rotation[3].w);
		this->get_component<thebanana::rigidbody>().apply_force(rotation * glm::vec4(translate, 1.f));
#ifdef _DEBUG
	}
#endif
}
void player_behavior::move(float yaw_offset, glm::vec3& translate, const float speed) {
	glm::vec2& last_angle = *this->get_property<glm::vec2>("last_camera_angle");
	thebanana::gameobject* camera = *this->get_property<thebanana::gameobject*>("camera");
	glm::vec2 current_angle = last_angle - (camera->get_component<thebanana::native_script_component>().get_script<camera_behavior>()->get_angle() + glm::vec2(0.f, yaw_offset));
	last_angle = camera->get_component<thebanana::native_script_component>().get_script<camera_behavior>()->get_angle() + glm::vec2(0.f, yaw_offset);
	this->get_transform().rotate(current_angle.y, glm::vec3(0.f, 1.f, 0.f));
	translate.z += speed;
}