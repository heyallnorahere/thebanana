#pragma once
#include "transform.h"
#include "component.h"
namespace thebanana {
#define ROOT ((::thebanana::gameobject*)NULL)
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
		template<typename T> T* add_object(T* obj);
		void remove_object(size_t index);
		transform get_absolute_transform();
		std::string& get_nickname();
		scene* get_scene();
		game* get_game();
		template<typename _Ty> _Ty& add_component();
		template<typename _Ty> _Ty& get_component(size_t index = 0);
		template<typename _Ty> void remove_component(size_t index = 0);
		template<typename _Ty> size_t get_number_components();
		template<typename _Ty> bool has_component();
		const component::properties_t& get_properties();
		void on_collision(gameobject* other);
		std::vector<std::string> get_tags() const;
		void add_tag(const std::string& tag);
		bool has_tag(const std::string& tag) const;
		unsigned long long get_uuid() const;
		void set_uuid(unsigned long long uuid);
		void find(unsigned long long uuid, gameobject*& ptr);
		bool is_initialized();
	protected:
		component::properties_t m_properties;
		void add_property(component::property_base* p);
		void init_component(component* c);
		template<typename _Ty> component::property<_Ty>* find_property(const std::string& name);
		void prepare_for_update();
		void prepare_for_render();
		void update_components();
		void render_components();
		void post_update();
		void post_render();
		std::string m_nickname;
		gameobject* m_parent;
		scene* m_scene;
		game* m_game;
		std::list<std::unique_ptr<gameobject>> m_children;
		transform m_transform;
		std::list<std::unique_ptr<component>> m_components;
		unsigned long long m_uuid;
	private:
		unsigned int last_collided_frame;
		bool initialized;
		void update_children();
		void render_children();
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