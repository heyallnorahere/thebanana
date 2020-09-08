#pragma once
#include "transform.h"
#include "component.h"
#define ROOT ((gameobject*)NULL)
class scene;
class game;
class gameobject {
public:
	gameobject();
	void init(gameobject* parent, scene* sc, game* g);
	virtual void update() = 0;
	virtual void render() = 0;
	virtual std::string get_model_name() = 0;
	virtual ~gameobject();
	const transform& get_transform() const;
	transform& get_transform();
	gameobject* get_parent() const;
	size_t get_relative_index() const;
	size_t get_absolute_index() const;
	size_t get_children_count() const;
	const gameobject* get_child(size_t index) const;
	gameobject* get_child(size_t index);
	void add_object(gameobject* obj);
	transform get_absolute_transform();
	std::string& get_nickname();
	scene* get_scene();
	game* get_game();
	template<typename _Ty> _Ty& add_component();
	template<typename _Ty> _Ty& get_component(size_t index = 0);
	template<typename _Ty> size_t get_number_components();
	int get_animation_index();
	void set_animation_index(int index);
	const component::properties_t& get_properties();
	void on_collision(gameobject* other);
	virtual float& get_last_walk_speed();
protected:
	component::properties_t m_properties;
	void add_property(component::property_base* p);
	template<typename _Ty> component::property<_Ty>* find_property(const std::string& name);
	void prepare_for_update();
	void prepare_for_render();
	void post_update();
	void post_render();
	void render_model(const std::string& name);
	std::string m_nickname;
	gameobject* m_parent;
	scene* m_scene;
	game* m_game;
	std::list<std::unique_ptr<gameobject>> m_children;
	transform m_transform;
	std::vector<std::unique_ptr<component>> m_components;
private:
	unsigned int last_collided_frame;
	bool initialized;
	int m_animation_index;
	void update_children();
	void render_children();
};
template<typename _Ty> inline _Ty& gameobject::add_component() {
	_Ty* c = new _Ty(this);
	if (this->initialized) c->initialize();
	this->m_components.push_back(std::unique_ptr<component>(c));
	return *c;
}
template<typename _Ty> inline _Ty& gameobject::get_component(size_t index) {
	if (typeid(_Ty).hash_code() == typeid(component).hash_code()) return (_Ty&)*(this->m_components[index % this->m_components.size()].get());
	std::vector<_Ty*> ptrs;
	for (auto& c : this->m_components) {
		if (typeid(*c).hash_code() == typeid(_Ty).hash_code()) {
			ptrs.push_back((_Ty*)c.get());
		}
	}
	if (ptrs.size() == 0) {
		return *(_Ty*)NULL;
	} else {
		return *ptrs[index % ptrs.size()];
	}
}
template<typename _Ty> inline size_t gameobject::get_number_components() {
	if (typeid(_Ty).hash_code() == typeid(component).hash_code()) return this->m_components.size();
	std::vector<_Ty*> ptrs;
	for (auto& c : this->m_components) {
		if (typeid(*c).hash_code() == typeid(_Ty).hash_code()) {
			ptrs.push_back((_Ty*)c.get());
		}
	}
	return ptrs.size();
}
template<typename _Ty> inline component::property<_Ty>* gameobject::find_property(const std::string& name) {
	component::property<_Ty>* result = NULL;
	for (auto& p : this->m_properties) {
		if (p->get_name() == name) {
			result = (component::property<_Ty>*)p.get();
			break;
		}
	}
	return result;
}
