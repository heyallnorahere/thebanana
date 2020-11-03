#pragma once
#include "gameobject.h"
namespace thebanana {
	class game;
	class player;
	class camera;
	class scene {
	public:
		scene(game* g);
		template<typename T> T* add_object(T* obj);
		void update();
		void render();
		size_t get_children_count() const;
		const gameobject* get_child(size_t index) const;
		gameobject* get_child(size_t index);
		opengl_shader_library::shader* get_shader() const;
		player* get_player();
		camera* get_camera();
	private:
		player* m_player;
		camera* m_camera;
		game* m_game;
		std::list<std::unique_ptr<gameobject>> m_children;
		std::unique_ptr<opengl_shader_library::shader> m_shader;
		friend class scene_serializer;
	};
	template<typename T> inline T* scene::add_object(T* obj) {
		obj->init(ROOT, this, this->m_game);
		this->m_children.push_back(std::unique_ptr<gameobject>(obj));
		return obj;
	}
}