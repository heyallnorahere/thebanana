#pragma once
#include "gameobject.h"
class game;
class player;
class camera;
class scene {
public:
	scene(game* g);
	void add_object(gameobject* obj);
	void update();
	void render();
	size_t get_children_count() const;
	const gameobject* get_child(size_t index) const;
	gameobject* get_child(size_t index);
	opengl_shader_library::shader* get_shader() const;
	player* get_player();
	camera* get_camera();
private:
	void update_camera_angle();
	player* m_player;
	camera* m_camera;
	game* m_game;
	std::list<std::unique_ptr<gameobject>> m_children;
	std::unique_ptr<opengl_shader_library::shader> m_shader;
};