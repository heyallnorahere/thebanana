#include "pch.h"
#include "component.h"
#include "gameobject.h"
#include "scene.h"
#include "game.h"
#include "util.h"
#include "debug_tools.h"
#include "internal_util.h"
namespace thebanana {
	component::component(gameobject* obj) : parent(obj) {
		this->uuid = generate_uuid();
	}
	void component::initialize() { }
	void component::pre_update() { }
	void component::update() { }
	void component::post_update() { }
	void component::pre_render() { }
	void component::render() { }
	void component::post_render() { }
	void component::on_collision(gameobject* other) { }
	void component::clean_up() { }
	const component::properties_t& component::get_properties() const {
		return this->properties;
	}
	gameobject* component::get_parent() {
		return this->parent;
	}
	unsigned long long component::get_uuid() const {
		return this->uuid;
	}
	void component::set_uuid(unsigned long long uuid) {
		this->uuid = uuid;
	}
	void component::add_property(property_base* p) {
		this->properties.push_back(std::unique_ptr<property_base>(p));
	}
	void component::remove_property(const std::string& name) {
		std::unique_ptr<property_base>* prop = NULL;
		for (auto& p : this->properties) {
			if (p->get_name() == name) {
				prop = &p;
				break;
			}
		}
		assert(prop);
		this->properties.remove(*prop);
	}
	transform& component::get_transform() {
		return this->parent->get_transform();
	}
	component::property_base::property_base(const std::string& name, size_t size) {
		this->ptr = malloc(size);
		this->name = name;
		this->selection_window_open = false;
		this->selection_window_temp = NULL;
	}
	const std::string& component::property_base::get_name() {
		return this->name;
	}
	component::property_base::~property_base() { }
	void* component::property_base::get_ptr() {
		return this->ptr;
	}
	bool component::property_base::is_selection_window_open() const {
		return this->selection_window_open;
	}
	debug_component::debug_component(gameobject* obj) : component(obj), flash_start_time(0.0), flash_end_time(0.0) {
		this->add_property(new property<double>(2.0, "Flash rate"));
		this->add_property(new property<double>(0.1, "Flash length"));
		this->add_property(new property<gameobject*>(NULL, "Test object"));
	}
	namespace debug {
		extern gameobject* current_selected_gameobject;
	}
	void debug_component::pre_render() {
		this->parent->get_scene()->get_shader()->get_uniforms().vec3("fill_color", glm::vec3(1.f, 0.5f, 0.f));
		property<double>* flash_rate = this->find_property<double>("Flash rate");
		property<double>* fl = this->find_property<double>("Flash length");
		double time = CURRENT_TIME(double);
		double delay_length = flash_rate ? 1.0 / *flash_rate->get_value() : 0.5;
		double flash_length = fl ? *fl->get_value() : 0.1;
		bool solid = false;
		if (this->flash_end_time == 0.0 || this->flash_end_time < this->flash_start_time) {
			if (time - this->flash_start_time >= flash_length) {
				this->flash_end_time = time;
			}
			else {
				solid = true;
			}
		}
		else {
			if (time - this->flash_start_time >= delay_length) {
				this->flash_start_time = time;
			}
		}
		this->parent->get_scene()->get_shader()->get_uniforms()._int("solid_color", solid && this->parent == debug::current_selected_gameobject);
	}
	component::property_base::dropdown::dropdown(const std::vector<std::string>& items, int initial_index) : m_items(items), m_index(initial_index) { }
	component::property_base::dropdown::dropdown(const std::vector<const char*>& items, int initial_index) : m_index(initial_index) {
		for (const char* str : items) {
			this->m_items.push_back(str);
		}
	}
	int* component::property_base::dropdown::get_index_ptr() {
		return &this->m_index;
	}
	const std::vector<std::string>& component::property_base::dropdown::get_items() const {
		return this->m_items;
	}
	void component::property_base::dropdown::set_items(const std::vector<std::string>& items) {
		this->m_items = items;
	}
	component::property_base::read_only_text::read_only_text(const std::string& text) : text(text) { }
	std::string& component::property_base::read_only_text::get_text() { return this->text; }
	unsigned long long get_uuid(gameobject* obj) {
		return obj->get_uuid();
	}
}