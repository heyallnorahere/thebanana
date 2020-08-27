#include "pch.h"
#include "gameobject.h"
#include "game.h"
#include "scene.h"
#include "model_registry.h"
void gameobject::init(gameobject* parent, scene* sc, game* g) {
	this->m_parent = parent;
	this->m_scene = sc;
	this->m_game = g;
	this->m_animation_index = -1;
}
gameobject::~gameobject() {
}
const transform& gameobject::get_transform() const {
	return this->m_transform;
}
transform& gameobject::get_transform() {
	return this->m_transform;
}
gameobject* gameobject::get_parent() const {
	return this->m_parent;
}
size_t gameobject::get_relative_index() const {
	size_t index = std::string::npos;
	if (this->m_parent != ROOT) {
		for (size_t i = 0; i < this->m_parent->get_children_count(); i++) {
			if (this == this->m_parent->get_child(i)) {
				index = i;
				break;
			}
		}
	} else {
		for (size_t i = 0; i < this->m_scene->get_children_count(); i++) {
			if (this == this->m_scene->get_child(i)) {
				index = i;
				break;
			}
		}
	}
	return index;
}
size_t gameobject::get_absolute_index() const {
	size_t index = this->get_relative_index();
	if (this->m_parent != ROOT) {
		index += this->m_parent->get_absolute_index();
	}
	return index;
}
size_t gameobject::get_children_count() const {
	return this->m_children.size();
}
const gameobject* gameobject::get_child(size_t index) const {
	auto it = this->m_children.begin();
	std::advance(it, index);
	const auto& c = *it;
	return c.get();
}
gameobject* gameobject::get_child(size_t index) {
	auto it = this->m_children.begin();
	std::advance(it, index);
	const auto& c = *it;
	return c.get();
}
void gameobject::add_object(gameobject* obj) {
	obj->init(this, this->m_scene, this->m_game);
	this->m_children.push_back(std::unique_ptr<gameobject>(obj));
}
transform gameobject::get_absolute_transform() {
	if (this->m_parent == ROOT) {
		return this->m_transform;
	} else {
		return this->m_transform * this->m_parent->get_absolute_transform();
	}
}
void gameobject::prepare_for_update() {
	this->update_children();
	std::stringstream ss;
	ss << "updating gameobject; relative id: " << this->get_relative_index() << ", absolute id: " << this->get_absolute_index() << "\n";
	std::string output = ss.str();
	OutputDebugStringA(output.c_str());
}
void gameobject::prepare_for_render() {
	this->render_children();
	this->m_scene->get_shader()->get_uniforms().mat4("model", this->get_absolute_transform().get_matrix());
}
void gameobject::render_model(const std::string& name) {
	transform model_transform = this->m_game->get_model_registry()->get_transform(name);
	model_transform *= this->m_game->get_model_registry()->get_scene(name)->mRootNode->mTransformation;
	this->m_scene->get_shader()->get_uniforms().mat4("model_transform", model_transform.get_matrix());
	this->m_game->get_model_registry()->draw(name, CURRENT_TIME(double), this->m_animation_index, this->m_scene->get_shader());
}
void gameobject::update_children() {
	for (auto& c : this->m_children) {
		c->update();
	}
}
void gameobject::render_children() {
	for (auto& c : this->m_children) {
		c->render();
	}
}
