#include "pch.h"
#include "component.h"
#include "gameobject.h"
#include "scene.h"
#include "game.h"
#include "util.h"
#include "debug_tools.h"
component::component(gameobject* obj) : parent(obj) { }
void component::initialize() { }
void component::pre_update() { }
void component::post_update() { }
void component::pre_render() { }
void component::post_render() { }
void component::clean_up() { }
const component::properties_t& component::get_properties() const {
	return this->properties;
}
void component::add_property(property_base* p) {
	this->properties.push_back(std::unique_ptr<property_base>(p));
}
transform& component::get_transform() {
	return this->parent->get_transform();
}
int component::get_animation_index() {
	return this->parent->get_animation_index();
}
void component::set_animation_index(int index) {
	this->parent->set_animation_index(index);
}
test_component::test_component(gameobject* obj) : component(obj) {
	this->add_property(new property<int>(42, "x"));
	this->add_property(new property<glm::vec3>(glm::vec3(1.f, -2.f, 3.4f), "y"));
	this->add_property(new property<std::string>("hello", "z"));
}
void test_component::print_properties() {
	property<int>* x = this->find_property<int>("x");
	if (x) {
		std::stringstream ss;
		ss << "x: " << *x->get_value() << "\n";
		log_print(ss.str());
	}
	property<glm::vec3>* y = this->find_property<glm::vec3>("y");
	if (y) {
		std::stringstream ss;
		ss << "y: " << y->get_value()->x << ", " << y->get_value()->y << ", " << y->get_value()->z << "\n";
		log_print(ss.str());
	}
	property<std::string>* z = this->find_property<std::string>("z");
	if (z) {
		std::stringstream ss;
		ss << "z: " << *z->get_value() << "\n";
		log_print(ss.str());
	}
}
component::property_base::property_base(const std::string& name, size_t size) {
	this->ptr = malloc(size);
	this->name = name;
}
const std::string& component::property_base::get_name() {
	return this->name;
}
component::property_base::~property_base() { }
void* component::property_base::get_ptr() {
	return this->ptr;
}
debug_component::debug_component(gameobject* obj) : component(obj), flash_start_time(0.0), flash_end_time(0.0) {
	this->add_property(new property<double>(2.0, "flash rate"));
	this->add_property(new property<double>(0.1, "flash length"));
}
extern gameobject* current_selected_gameobject;
void debug_component::pre_render() {
	this->parent->get_scene()->get_shader()->get_uniforms().vec3("fill_color", glm::vec3(1.f, 0.5f, 0.f));
	property<double>* flash_rate = this->find_property<double>("flash rate");
	property<double>* fl = this->find_property<double>("flash length");
	double time = CURRENT_TIME(double);
	double delay_length = flash_rate ? 1.0 / *flash_rate->get_value() : 0.5;
	double flash_length = fl ? *fl->get_value() : 0.1;
	bool solid = false;
	if (this->flash_end_time == 0.0 || this->flash_end_time < this->flash_start_time) {
		if (time - this->flash_start_time >= flash_length) {
			this->flash_end_time = time;
		} else {
			solid = true;
		}
	} else {
		if (time - this->flash_start_time >= delay_length) {
			this->flash_start_time = time;
		}
	}
	this->parent->get_scene()->get_shader()->get_uniforms()._int("solid_color", solid && this->parent == current_selected_gameobject);
}
animation_component::animation_component(gameobject* obj) : component(obj), animation_start_time(0.0), repeat(false) { }
void animation_component::post_update() {
	if (this->get_animation_index() > -1) {
		double current_time = this->get_animation_time();
		const aiScene* s = this->parent->get_game()->get_model_registry()->get_scene(this->parent->get_model_name());
		if (!s) return;
		aiAnimation* animation = s->mAnimations[this->get_animation_index()];
		double length_in_sec = animation->mDuration / animation->mTicksPerSecond;
		if (!this->repeat && current_time > length_in_sec) {
			this->stop_animation();
		}
	}
}
void animation_component::start_animation(int index, bool repeat) {
	this->set_animation_index(index);
	this->animation_start_time = CURRENT_TIME(double);
	this->repeat = repeat;
}
void animation_component::start_animation(const std::string& name, bool repeat) {
	const aiScene* scene = this->parent->get_game()->get_model_registry()->get_scene(this->parent->get_model_name());
	for (int i = 0; i < scene->mNumAnimations; i++) {
		std::string animation_name(scene->mAnimations[i]->mName.data, scene->mAnimations[i]->mName.length);
		if (name == animation_name.substr(animation_name.find_last_of('|') + 1)) {
			this->start_animation(i, repeat);
			break;
		}
	}
}
void animation_component::stop_animation() {
	this->set_animation_index(-1);
	this->repeat = false;
}
double animation_component::get_animation_time() {
	return CURRENT_TIME(double) - this->animation_start_time;
}
