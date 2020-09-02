#include "pch.h"
#include "rigidbody.h"
#include "gameobject.h"
#include "debug_tools.h"
#include "game.h"
#include "player.h"
std::vector<rigidbody*> rigidbodies;
rigidbody::rigidbody(gameobject* obj) : component(obj) {
	log_print("created rigidbody under gameobject " + std::string(typeid(*obj).name()));
	this->add_property(new property<bool>(true, "interactable"));
	this->coll = NULL;
	this->last_frame_model_name = "";
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
	if (this->is_player) {
		for (auto ptr : rigidbodies) {
			if (ptr == this) continue;
			glm::vec4 my_pos = this->parent->get_transform().get_matrix() * glm::vec4(0.f, 0.f, 0.f, 1.f);
			glm::vec4 other_pos = ptr->parent->get_transform().get_matrix() * glm::vec4(0.f, 0.f, 0.f, 1.f);
			glm::vec4 difference = my_pos - other_pos;
			if (difference.length() > 100.f) continue;
			if (this->coll) this->coll->detect_collision(ptr);
		}
	}
}
void rigidbody::clean_up() {
	delete this->coll;
}
gameobject* rigidbody::get_parent() {
	return this->parent;
}
model_registry::model_vertex_data rigidbody::get_vertex_data() {
	return this->vertex_data;
}
