#pragma once
#include "gameobject.h"
#include "banana_api.h"
namespace thebanana {
	class game;
	class scene {
	public:
		BANANA_API scene(game* g);
		BANANA_API ~scene();
		template<typename T> T* add_object(T* obj);
		BANANA_API void remove_object(size_t index);
		BANANA_API void clear();
		BANANA_API void update();
		BANANA_API void render();
		BANANA_API size_t get_children_count() const;
		BANANA_API const gameobject* get_child(size_t index) const;
		BANANA_API gameobject* get_child(size_t index);
		BANANA_API opengl_shader_library::shader* get_shader() const;
		BANANA_API void set_shader_name(const std::string& shader_name);
		BANANA_API game* get_game();
		BANANA_API gameobject* find(unsigned long long uuid);
		BANANA_API gameobject* find_main_camera();
	private:
		game* m_game;
		std::list<std::unique_ptr<gameobject>> m_children;
		opengl_shader_library::shader* m_shader;
		friend class scene_serializer;
	};
	template<typename T> inline T* scene::add_object(T* obj) {
		if (!obj->is_initialized()) obj->init(ROOT, this, this->m_game);
		this->m_children.push_back(std::unique_ptr<gameobject>(obj));
		return obj;
	}
}