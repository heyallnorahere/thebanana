#include "pch.h"
#include "gameobject.h"
#include "game.h"
#include "scene.h"
#include "model_registry.h"
#include "debug_tools.h"
#include "components/tag_component.h"
#include "components/native_script_component.h"
#include "internal_util.h"
#include "particlesystem/particle_component.h"
namespace thebanana {
	gameobject::gameobject() : last_collided_frame(0) {
#ifdef _DEBUG
		this->add_component<debug_component>();
#endif
		this->initialized = false;
		this->m_nickname = "object";
		this->m_uuid = generate_uuid();
	}
	void gameobject::init(gameobject* parent, scene* sc, game* g) {
		this->m_parent = parent;
		this->m_scene = sc;
		this->m_game = g;
		for (auto& c : this->m_components) {
			this->init_component(c.get());
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
		}
		else {
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
	void gameobject::remove_object(size_t index) {
		auto it = this->m_children.begin();
		std::advance(it, index);
		auto& obj = *it;
		this->m_children.remove(obj);
	}
	transform gameobject::get_absolute_transform() {
		if (this->m_parent == ROOT) {
			return this->m_transform;
		}
		else {
			return this->m_parent->get_absolute_transform() * this->m_transform;
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
	const component::properties_t& gameobject::get_properties() {
		return this->m_properties;
	}
	void gameobject::on_collision(gameobject* other) {
		if (this->last_collided_frame == this->m_game->get_current_frame()) return;
		for (auto& c : this->m_components) {
			c->on_collision(other);
		}
		this->last_collided_frame = this->m_game->get_current_frame();
	}
	std::vector<std::string> gameobject::get_tags() const {
		std::vector<std::string> tags;
		for (size_t i = 0; i < ((gameobject*)this)->get_number_components<tag_component>(); i++) {
			tags.push_back(((gameobject*)this)->get_component<tag_component>(i).get_tag());
		}
		return tags;
	}
	void gameobject::add_tag(const std::string& tag) {
		this->add_component<tag_component>().set_tag(tag);
	}
	bool gameobject::has_tag(const std::string& tag) const {
		for (size_t i = 0; i < ((gameobject*)this)->get_number_components<tag_component>(); i++) {
			if (tag == ((gameobject*)this)->get_component<tag_component>().get_tag()) return true;
		}
		return false;
	}
	unsigned long long gameobject::get_uuid() const {
		return this->m_uuid;
	}
	void gameobject::set_uuid(unsigned long long uuid) {
		this->m_uuid = uuid;
	}
	void gameobject::find(unsigned long long uuid, gameobject*& ptr) {
		if (this->m_uuid == uuid) {
			ptr = this;
		} else {
			for (auto& c : this->m_children) {
				c->find(uuid, ptr);
				if (ptr) break;
			}
		}
	}
	bool gameobject::is_initialized() {
		return this->initialized;
	}
	void gameobject::add_property(component::property_base* p) {
		p->set_parent(NULL);
		this->m_properties.push_back(std::unique_ptr<component::property_base>(p));
	}
	void gameobject::init_component(component* c) {
		for (auto& p : c->get_properties()) {
			((std::unique_ptr<component::property_base>&)p)->set_game(this->m_game);
		}
		c->initialize();
	}
	void gameobject::prepare_for_update() {
		this->update_children();
		for (auto& c : this->m_components) {
			c->pre_update();
		}
	}
	void gameobject::prepare_for_render() {
		this->render_children();
		for (auto& c : this->m_components) {
			c->pre_render();
		}
		this->m_scene->get_shader()->get_uniforms().mat4("model", this->get_absolute_transform().get_matrix());
	}
	void gameobject::update_components() {
		for (auto& c : this->m_components) {
			c->update();
		}
	}
	void gameobject::render_components() {
		for (auto& c : this->m_components) {
			c->render();
		}
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
	bool gameobject::is_script(component* c) {
		return (typeid(*c).hash_code() == typeid(native_script_component).hash_code());
	}
	size_t gameobject::script_hash(component* c) {
		native_script_component* nsc = (native_script_component*)c;
		return typeid(*nsc->get_script<script>()).hash_code();
	}
	void* gameobject::get_script_from_component(component* c) {
		native_script_component* nsc = (native_script_component*)c;
		return nsc->get_script<void>();
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
}