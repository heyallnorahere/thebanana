#include "pch.h"
#include "rigidbody.h"
#include "gameobject.h"
#include "debug_tools.h"
#include "game.h"
#include "player.h"
std::vector<rigidbody*> rigidbodies;
rigidbody::rigidbody(gameobject* obj) : component(obj) {
	this->add_property(new property<bool>(false, "gravity"));
	this->add_property(new property<float>(1.f, "gravity multiplier"));
	log_print("created rigidbody under gameobject " + std::string(typeid(*obj).name()));
	this->coll = NULL;
	this->last_frame_model_name = "";
	this->check_for_collisions = false;
	this->velocity = glm::vec3(0.f);
	this->acceleration = glm::vec3(0.f);
}
void rigidbody::initialize() {
	rigidbodies.push_back(this);
}
void rigidbody::pre_update() {
	if (this->parent->get_model_name() != this->last_frame_model_name) {
		this->vertex_data = this->parent->get_game()->get_model_registry()->get_vertex_data(this->parent->get_model_name());
		this->last_frame_model_name = this->parent->get_model_name();
	}
}
void rigidbody::post_update() {
	property<bool>* _gravity = this->find_property<bool>("gravity");
	property<float>* _multiplier = this->find_property<float>("gravity multiplier");
	if (_gravity && _multiplier) {
		float multiplier = *_multiplier->get_value();
		if (*_gravity->get_value()) {
			this->acceleration += gravity * multiplier;
		}
	}
	this->velocity += this->acceleration;
	this->parent->get_transform().move(this->velocity);
	this->parent->get_last_walk_speed() += glm::length(this->velocity);
	this->acceleration = glm::vec3(0.f);
	this->num_collisions = 0;
	this->shift_delta = glm::vec3(0.f);
	if (this->coll && this->check_for_collisions) {
		for (auto ptr : rigidbodies) {
			if (ptr == this) continue;
			glm::vec4 my_pos = this->parent->get_transform().get_matrix() * glm::vec4(0.f, 0.f, 0.f, 1.f);
			glm::vec4 other_pos = ptr->parent->get_transform().get_matrix() * glm::vec4(0.f, 0.f, 0.f, 1.f);
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
	this->acceleration += force;
}
