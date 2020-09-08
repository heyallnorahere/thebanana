#include "pch.h"
#include "rigidbody.h"
#include "gameobject.h"
#include "debug_tools.h"
#include "game.h"
#include "player.h"
std::vector<rigidbody*> rigidbodies;
rigidbody::rigidbody(gameobject* obj) : component(obj) {
	log_print("created rigidbody under gameobject " + std::string(typeid(*obj).name()));
	this->coll = NULL;
	this->last_frame_model_name = "";
	this->check_for_collisions = false;
}
void rigidbody::initialize() {
	rigidbodies.push_back(this);
	this->is_player = typeid(*this->parent).hash_code() == typeid(player).hash_code();
}
void rigidbody::pre_update() {
	if (this->parent->get_model_name() != this->last_frame_model_name) {
		this->vertex_data = this->parent->get_game()->get_model_registry()->get_vertex_data(this->parent->get_model_name());
		this->last_frame_model_name = this->parent->get_model_name();
	}
}
void rigidbody::post_update() {
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
		if (typeid(*this->coll).hash_code() == typeid(mlfarrel_model).hash_code()) {
			float last_walk_speed = this->parent->get_last_walk_speed();
			if (glm::length(this->shift_delta) > last_walk_speed) {
				this->shift_delta = glm::normalize(this->shift_delta);
				this->shift_delta *= last_walk_speed * 1.1f;
			}
			this->parent->get_transform().move(this->shift_delta);
		}
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
