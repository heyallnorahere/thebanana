#include "pch.h"
#include "physics/rigidbody.h"
#include "gameobject.h"
#include "debug_tools.h"
#include "game.h"
namespace thebanana {
	const float drag = 0.05f;
	rigidbody::rigidbody(gameobject* obj) : component(obj) {
		this->add_property(new property<bool>(false, "Gravity"));
		this->add_property(new property<glm::vec3>(glm::vec3(0.f, -1.f, 0.f), "Gravity value"));
		this->add_property(new property<float>(1.f, "Mass"));
		this->add_property(new property<float>(drag, "Drag"));
		this->add_property(new property<property_base::dropdown>(property_base::dropdown(std::vector<std::string>({ "None", "Collide" })), "Collision type"));
		this->add_property(new property<std::string>("", "Collision mesh name"));
		this->coll = NULL;
		this->last_frame_model_name = "";
		this->check_for_collisions = false;
		this->velocity = glm::vec3(0.f);
		this->acceleration = glm::vec3(0.f);
		this->last_frame_pos = glm::vec3(0.f);
		this->has_cap = false;
	}
	void rigidbody::initialize() {
		this->parent->get_game()->get_rigidbody_list().push_back(this);
	}
	void rigidbody::pre_update() {
		if (this->get_collision_model_name() != this->last_frame_model_name) {
			this->vertex_data = this->parent->get_game()->get_model_registry()->get_vertex_data(this->get_collision_model_name());
			this->last_frame_model_name = this->get_collision_model_name();
		}
	}
	void rigidbody::post_update() {
		property<bool>* _gravity = this->find_property<bool>("Gravity");
		property<glm::vec3>* _value = this->find_property<glm::vec3>("Gravity value");
		property<float>* _mass = this->find_property<float>("Mass");
		if (_gravity && _value && _mass) {
			glm::vec3 value = *_value->get_value();
			if (*_gravity->get_value()) {
				this->acceleration += value * (*_mass->get_value());
			}
		}
		this->velocity += this->acceleration;
		if (this->has_cap) if (glm::length(this->velocity) > this->speed_cap) {
			this->velocity = glm::normalize(this->velocity) * this->speed_cap;
		}
		this->parent->get_transform().move(this->velocity);
		property<float>* _drag = this->find_property<float>("Drag");
		this->velocity *= 1.f - (_drag ? *_drag->get_value() : drag);
		this->acceleration = glm::vec3(0.f);
		this->num_collisions = 0;
		this->shift_delta = glm::vec3(0.f);
		if (this->coll && this->check_for_collisions) {
			for (auto ptr : this->parent->get_game()->get_rigidbody_list()) {
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
		this->last_frame_pos = this->get_transform();
	}
	void rigidbody::on_collision(gameobject* other) {
		property_base::dropdown* collision_type = this->get_property<property_base::dropdown>("Collision type");
		if (this->num_collisions != 0 && this->check_for_collisions && (*collision_type->get_index_ptr() == 1)) {
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
		this->set_property("Collision mesh name", model_name);
		return *this;
	}
	std::string rigidbody::get_collision_model_name() {
		return *this->get_property<std::string>("Collision mesh name");
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
		return glm::length((glm::vec3)this->get_transform() - this->last_frame_pos);
	}
	collider* rigidbody::get_collider() const {
		return this->coll;
	}
}