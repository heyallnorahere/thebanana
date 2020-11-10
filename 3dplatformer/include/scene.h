#pragma once
#include "gameobject.h"
namespace thebanana {
	class game;
	class scene {
	public:
		scene(game* g);
		template<typename T> T* add_object(T* obj);
		void clear();
		void update();
		void render();
		size_t get_children_count() const;
		const gameobject* get_child(size_t index) const;
		gameobject* get_child(size_t index);
		opengl_shader_library::shader* get_shader() const;
		void set_shader_name(const std::string& shader_name);
		game* get_game();
	private:
		game* m_game;
		std::list<std::unique_ptr<gameobject>> m_children;
		opengl_shader_library::shader* m_shader;
		friend class scene_serializer;
	};
	template<typename T> inline T* scene::add_object(T* obj) {
		obj->init(ROOT, this, this->m_game);
		this->m_children.push_back(std::unique_ptr<gameobject>(obj));
		return obj;
	}
}