#pragma once
#include "transform.h"
#define ROOT ((gameobject*)NULL)
class scene;
class game;
class gameobject {
public:
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
	void add_object(gameobject* obj);
protected:
	void prepare_for_update();
	void prepare_for_render();
	void render_model(const std::string& name);
	gameobject* m_parent;
	scene* m_scene;
	game* m_game;
	std::list<std::unique_ptr<gameobject>> m_children;
	transform m_transform;
private:
	int m_animation_index;
	void update_children();
	void render_children();
};