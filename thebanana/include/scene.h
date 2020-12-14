#pragma once
#include "gameobject.h"
#include "components/light_component.h"
#include "banana_api.h"
namespace opengl_shader_library {
	class shader;
}
namespace thebanana {
	class game;
	class scene {
	public:
		struct shadow_settings {
			float left, right, bottom, top;
			float near_plane, far_plane;
		};
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
		BANANA_API opengl_shader_library::shader* get_depth_shader() const;
		BANANA_API void set_shader_name(const std::string& shader_name);
		BANANA_API void set_depth_shader_name(const std::string& shader_name);
		BANANA_API game* get_game();
		BANANA_API gameobject* find(unsigned long long uuid);
		BANANA_API gameobject* find_main_camera();
		BANANA_API std::vector<light_component::light_data> get_lights();
		BANANA_API shadow_settings& get_shadow_settings();
		BANANA_API void set_shadow_defaults();
		template<typename T> std::vector<T*> find_all_instances_of_component();
		template<typename T> T& find_component(size_t index = 0);
	private:
		void render_scene();
		game* m_game;
		std::list<std::unique_ptr<gameobject>> m_children;
		opengl_shader_library::shader* m_shader;
		opengl_shader_library::shader* m_depth_shader;
		shadow_settings m_shadow_settings;
		friend class scene_serializer;
	};
	template<typename T> inline T* scene::add_object(T* obj) {
		if (!obj->is_initialized()) obj->init(ROOT, this, this->m_game);
		this->m_children.push_back(std::unique_ptr<gameobject>(obj));
		return obj;
	}
	template<typename T> inline void add_instances(gameobject* object, std::vector<T*>& instances) {
		for (size_t i = 0; i < object->get_number_components<T>(); i++) {
			instances.push_back(&object->get_component<T>(i));
		}
		for (size_t i = 0; i < object->get_children_count(); i++) {
			add_instances(object->get_child(i), instances);
		}
	}
	template<typename T> inline std::vector<T*> scene::find_all_instances_of_component() {
		std::vector<T*> instances;
		for (auto& object : this->m_children) {
			add_instances(object.get(), instances);
		}
		return instances;
	}
	template<typename T> inline T& scene::find_component(size_t index) {
		return *this->find_all_instances_of_component<T>()[index];
	}
}