#pragma once
#include "transform.h"
#include "component.h"
#include "banana_api.h"
namespace thebanana {
#define ROOT ((::thebanana::gameobject*)NULL)
	class scene;
	class game;
	class gameobject {
	public:
		BANANA_API gameobject();
		BANANA_API void init(gameobject* parent, scene* sc, game* g);
		BANANA_API virtual void update() = 0;
		BANANA_API virtual void render() = 0;
		BANANA_API virtual ~gameobject();
		BANANA_API const transform& get_transform() const;
		BANANA_API transform& get_transform();
		BANANA_API gameobject* get_parent() const;
		BANANA_API size_t get_relative_index() const;
		BANANA_API size_t get_absolute_index() const;
		BANANA_API size_t get_children_count() const;
		BANANA_API const gameobject* get_child(size_t index) const;
		BANANA_API gameobject* get_child(size_t index);
		template<typename T> T* add_object(T* obj);
		BANANA_API void remove_object(size_t index);
		BANANA_API transform get_absolute_transform();
		BANANA_API std::string& get_nickname();
		BANANA_API scene* get_scene();
		BANANA_API game* get_game();
		template<typename _Ty> _Ty& add_component();
		template<typename _Ty> _Ty& get_component(size_t index = 0);
		template<typename _Ty> _Ty* find_script();
		template<typename _Ty> void remove_component(size_t index = 0);
		template<typename _Ty> size_t get_number_components();
		template<typename _Ty> bool has_component();
		BANANA_API const component::properties_t& get_properties();
		BANANA_API void on_collision(gameobject* other);
		BANANA_API std::vector<std::string> get_tags() const;
		BANANA_API void add_tag(const std::string& tag);
		BANANA_API bool has_tag(const std::string& tag) const;
		BANANA_API unsigned long long get_uuid() const;
		BANANA_API void set_uuid(unsigned long long uuid);
		BANANA_API void find(unsigned long long uuid, gameobject*& ptr);
		BANANA_API bool is_initialized();
	protected:
		component::properties_t m_properties;
		BANANA_API void add_property(component::property_base* p);
		BANANA_API void init_component(component* c);
		template<typename _Ty> component::property<_Ty>* find_property(const std::string& name);
		BANANA_API void prepare_for_update();
		BANANA_API void prepare_for_render();
		BANANA_API void update_components();
		BANANA_API void render_components();
		BANANA_API void post_update();
		BANANA_API void post_render();
		std::string m_nickname;
		gameobject* m_parent;
		scene* m_scene;
		game* m_game;
		std::list<std::unique_ptr<gameobject>> m_children;
		transform m_transform;
		std::list<std::unique_ptr<component>> m_components;
		unsigned long long m_uuid;
	private:
		BANANA_API bool is_script(component* c);
		BANANA_API size_t script_hash(component* c);
		BANANA_API void* get_script_from_component(component* c);
		unsigned int last_collided_frame;
		bool initialized;
		BANANA_API void update_children();
		BANANA_API void render_children();
		friend class scene_serializer;
	};
	template<typename T> inline T* gameobject::add_object(T* obj) {
		if (!obj->is_initialized()) obj->init(this, this->m_scene, this->m_game);
		this->m_children.push_back(std::unique_ptr<gameobject>(obj));
		return obj;
	}
	template<typename _Ty> inline _Ty& gameobject::add_component() {
		_Ty* c = new _Ty(this);
		if (this->initialized) this->init_component(c);
		this->m_components.push_back(std::unique_ptr<component>(c));
		return *c;
	}
	template<typename _Ty> inline _Ty& gameobject::get_component(size_t index) {
		if (typeid(_Ty).hash_code() == typeid(component).hash_code()) {
			auto it = this->m_components.begin();
			std::advance(it, index % this->m_components.size());
			auto& p = *it;
			return (_Ty&)*p;
		}
		std::vector<_Ty*> ptrs;
		for (auto& c : this->m_components) {
			if (typeid(*c).hash_code() == typeid(_Ty).hash_code()) {
				ptrs.push_back((_Ty*)c.get());
			}
		}
		if (ptrs.size() == 0) {
			return *(_Ty*)NULL;
		}
		else {
			return *ptrs[index % ptrs.size()];
		}
	}
	template<typename _Ty> inline _Ty* gameobject::find_script() {
		_Ty* script = NULL;
		for (auto& c : this->m_components) {
			if (this->is_script(c.get())) {
				if (typeid(_Ty).hash_code() == this->script_hash(c.get())) {
					script = (_Ty*)this->get_script_from_component(c.get());
					break;
				}
			}
		}
		return script;
	}
	template<typename _Ty> inline void gameobject::remove_component(size_t index) {
		if (typeid(_Ty).hash_code() == typeid(component).hash_code()) {
			auto it = this->m_components.begin();
			std::advance(it, index % this->m_components.size());
			auto& p = *it;
			this->m_components.remove(p);
			return;
		}
		std::vector<size_t> indices;
		size_t current_index = 0;
		for (auto& c : this->m_components) {
			if (typeid(*c).hash_code() == typeid(_Ty).hash_code()) {
				indices.push_back(current_index);
			}
			current_index++;
		}
		if (indices.size() > 0) {
			auto it = this->m_components.begin();
			std::advance(it, indices[index % indices.size()]);
			auto& p = *it;
			this->m_components.remove(p);
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
	template<typename _Ty> inline bool gameobject::has_component() {
		return this->get_number_components<_Ty>() > 0;
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
	template<typename _Ty> inline bool component::has_component() {
		return this->parent->has_component<_Ty>();
	}
	template<typename _Ty> inline size_t component::get_number_components() {
		return this->parent->get_number_components<_Ty>();
	}
	template<typename _Ty> inline _Ty& component::get_component(size_t index) {
		return this->parent->get_component<_Ty>(index);
	}
}