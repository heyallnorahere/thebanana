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
	template<typename _Ty> _Ty& add_component();
	template<typename _Ty> _Ty& get_component();
protected:
	void prepare_for_update();
	void prepare_for_render();
	void render_model(const std::string& name);
	gameobject* m_parent;
	scene* m_scene;
	game* m_game;
	std::list<std::unique_ptr<gameobject>> m_children;
	transform m_transform;
	std::map<component::component_id, std::unique_ptr<component>> m_components;
private:
	bool initialized;
	int m_animation_index;
	void update_children();
	void render_children();
};
template<typename _Ty> inline _Ty& gameobject::add_component() {
	_Ty* c = new _Ty(this);
	if (this->initialized) c->initialize();
	this->m_components[_Ty::get_component_id()] = std::unique_ptr<component>(c);
	return this->get_component<_Ty>();
}
template<typename _Ty> inline _Ty& gameobject::get_component() {
	component::component_id id = _Ty::get_component_id();
	return (_Ty&)*this->m_components[id];
}
