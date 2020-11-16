#include "pch.h"
#include "physics/rigidbody.h"
#include "gameobject.h"
#include "debug_tools.h"
#include "game.h"
namespace thebanana {
	const float drag = 0.05f;
	rigidbody::rigidbody(gameobject* obj) : component(obj) {
		this->add_property(new property<bool>(false, "Gravity"));
		this->add_property(new property<float>(1.f, "Gravity multiplier"));
		this->add_property(new property<float>(1.f, "Mass"));
		this->add_property(new property<float>(drag, "Drag"));
		this->coll = NULL;
		this->last_frame_model_name = "";
		this->check_for_collisions = false;
		this->velocity = glm::vec3(0.f);
		this->acceleration = glm::vec3(0.f);
		this->last_velocity_length = 0.f;
		this->has_cap = false;
	}
	void rigidbody::initialize() {
		rigidbodies.push_back(this);
	}
	void rigidbody::pre_update() {
		if (this->collision_model_name != this->last_frame_model_name) {
			this->vertex_data = this->parent->get_game()->get_model_registry()->get_vertex_data(this->collision_model_name);
			this->last_frame_model_name = this->collision_model_name;
		}
	}
	void rigidbody::post_update() {
		property<bool>* _gravity = this->find_property<bool>("Gravity");
		property<float>* _multiplier = this->find_property<float>("Gravity multiplier");
		property<float>* _mass = this->find_property<float>("Mass");
		if (_gravity && _multiplier && _mass) {
			float multiplier = *_multiplier->get_value();
			if (*_gravity->get_value()) {
				this->acceleration += gravity * multiplier * (*_mass->get_value());
			}
		}
		this->velocity += this->acceleration;
		if (this->has_cap) if (glm::length(this->velocity) > this->speed_cap) {
			this->velocity = glm::normalize(this->velocity) * this->speed_cap;
		}
		this->parent->get_transform().move(this->velocity);
		this->last_velocity_length = glm::length(this->velocity);
		property<float>* _drag = this->find_property<float>("Drag");
		this->velocity *= 1.f - (_drag ? *_drag->get_value() : drag);
		this->acceleration = glm::vec3(0.f);
		this->num_collisions = 0;
		this->shift_delta = glm::vec3(0.f);
		if (this->coll && this->check_for_collisions) {
			for (auto ptr : rigidbodies) {
				if (ptr == this) continue;
				if (this->collision_tags.size() > 0) {
					bool has_tag = false;
					for (std::string tag : this->collision_tags) {
						if (ptr->parent->has_tag(tag)) {
							has_tag = true;
							break;
						}
					}
					if (!has_tag) continue;
				}
				glm::vec4 my_pos = this->parent->get_transform();
				glm::vec4 other_pos = ptr->parent->get_transform();
				glm::vec4 difference = my_pos - other_pos;
				if (difference.length() > 10.f) continue;
				if (this->coll->detect_collision(ptr)) {
					ptr->parent->on_collision(this->parent);
					this->parent->on_collision(ptr->parent);
				}
			}
		}
	}
	void rigidbody::on_collision(gameobject* other) {
		if (this->num_collisions != 0 && this->check_for_collisions) {
			this->coll->on_collision(other);
		}
	}
	void rigidbody::clean_up() {
		delete this->coll;
	}
	rigidbody& rigidbody::set_check_for_collisions(bool check) {
		this->check_for_collisions = check;
		return *this;
	}
	bool rigidbody::is_checking_for_collisions() {
		return this->check_for_collisions;
	}
	gameobject* rigidbody::get_parent() {
		return this->parent;
	}
	model_registry::model_vertex_data rigidbody::get_vertex_data() {
		return this->vertex_data;
	}
	glm::vec3& rigidbody::get_shift_delta() {
		return this->shift_delta;
	}
	int& rigidbody::get_num_collisions() {
		return this->num_collisions;
	}
	void rigidbody::apply_force(const glm::vec3& force) {
		property<float>* _mass = this->find_property<float>("Mass");
		float mass = _mass ? *_mass->get_value() : 1.f;
		this->acceleration += force / mass;
	}
	rigidbody& rigidbody::set_collision_model_name(const std::string& model_name) {
		this->collision_model_name = model_name;
		return *this;
	}
	std::string rigidbody::get_collision_model_name() {
		return this->collision_model_name;
	}
	rigidbody& rigidbody::set_collision_tags(const std::vector<std::string>& tags) {
		this->collision_tags = tags;
		return *this;
	}
	std::vector<std::string> rigidbody::get_collision_tags() {
		return this->collision_tags;
	}
	rigidbody& rigidbody::set_speed_cap(float cap) {
		this->speed_cap = cap;
		this->has_cap = true;
		return *this;
	}
	float rigidbody::get_speed_cap() {
		return this->has_cap ? this->speed_cap : 0.f;
	}
	float rigidbody::get_last_move_speed() {
		return this->last_velocity_length;
	}
	collider* rigidbody::get_collider() const {
		return this->coll;
	}
	const std::list<rigidbody*>& rigidbody::get_rigidbodies() {
		return rigidbodies;
	}
	std::list<rigidbody*> rigidbody::rigidbodies;
	glm::vec3 rigidbody::gravity = glm::vec3(0.f, -0.05f, 0.f);
}