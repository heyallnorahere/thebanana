#include "pch.h"
#include "gameobject.h"
#include "game.h"
#include "scene.h"
#include "model_registry.h"
#include "debug_tools.h"
gameobject::gameobject() {
#ifdef _DEBUG
	this->add_component<debug_component>();
#endif
	this->initialized = false;
	this->m_nickname = "object";
}
void gameobject::init(gameobject* parent, scene* sc, game* g) {
	this->m_parent = parent;
	this->m_scene = sc;
	this->m_game = g;
	this->m_animation_index = -1;
	for (auto& c : this->m_components) {
		c->initialize();
	}
	this->initialized = true;
}
gameobject::~gameobject() {
	for (auto& c : this->m_components) {
		c->clean_up();
	}
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
std::string& gameobject::get_nickname() {
	return this->m_nickname;
}
scene* gameobject::get_scene() {
	return this->m_scene;
}
game* gameobject::get_game() {
	return this->m_game;
}
int gameobject::get_animation_index() {
	return this->m_animation_index;
}
void gameobject::set_animation_index(int index) {
	this->m_animation_index = index;
}
const component::properties_t& gameobject::get_properties() {
	return this->m_properties;
}
void gameobject::add_property(component::property_base* p) {
	this->m_properties.push_back(std::unique_ptr<component::property_base> (p));
}
void gameobject::prepare_for_update() {
	this->update_children();
	std::stringstream ss;
	ss << "updating gameobject; relative id: " << this->get_relative_index() << ", absolute id: " << this->get_absolute_index() << "\n";
	for (auto& c : this->m_components) {
		c->pre_update();
	}
	log_print(ss.str());
}
void gameobject::prepare_for_render() {
	this->render_children();
	for (auto& c : this->m_components) {
		c->pre_render();
	}
	this->m_scene->get_shader()->get_uniforms().mat4("model", this->get_absolute_transform().get_matrix());
}
void gameobject::post_update() {
	for (auto& c : this->m_components) {
		c->post_update();
	}
}
void gameobject::post_render() {
	for (auto& c : this->m_components) {
		c->post_render();
	}
}
void gameobject::render_model(const std::string& name) {
	transform model_transform = this->m_game->get_model_registry()->get_transform(name);
	const aiScene* scene = this->m_game->get_model_registry()->get_scene(name);
	if (scene) model_transform *= scene->mRootNode->mTransformation;
	this->m_scene->get_shader()->get_uniforms().mat4("model_transform", model_transform.get_matrix());
	double animation_time = this->get_number_components<animation_component>() > 0 ? this->get_component<animation_component>().get_animation_time() : 0.0;
	this->m_game->get_model_registry()->draw(name, animation_time, this->m_animation_index, this->m_scene->get_shader());
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
