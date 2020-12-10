#pragma once
#include "component.h"
#include "gameobject.h"
#include "banana_api.h"
namespace thebanana {
	class native_script_component;
	class script {
	public:
		BANANA_API script(gameobject* object, native_script_component* nsc);
		BANANA_API virtual void initialize();
		BANANA_API virtual void pre_update();
		BANANA_API virtual void update();
		BANANA_API virtual void post_update();
		BANANA_API virtual void pre_render();
		BANANA_API virtual void render();
		BANANA_API virtual void post_render();
		BANANA_API virtual void on_collision(gameobject* other);
		BANANA_API virtual ~script();
		BANANA_API native_script_component* get_interface();
	protected:
		gameobject* parent;
		native_script_component* interface;
		template<typename _Ty> void set_property(const std::string& name, const _Ty& value);
		template<typename _Ty> _Ty* get_property(const std::string& name);
		template<typename _Ty> component::property<_Ty>* find_property(const std::string& name);
		BANANA_API void add_property(component::property_base* p);
		BANANA_API transform& get_transform();
		template<typename _Ty> _Ty& add_component();
		template<typename _Ty> size_t get_number_components();
		template<typename _Ty> bool has_component();
		template<typename _Ty> _Ty& get_component(size_t index = 0);
		template<typename _Ty> void remove_component(size_t index = 0);
	};
	class native_script_component : public component {
	public:
		BANANA_API native_script_component(gameobject* object);
		template<typename T> void bind();
		template<typename T> void bind(T* s);
		template<typename T> T* get_script();
		BANANA_API virtual void pre_update() override;
		BANANA_API virtual void update() override;
		BANANA_API virtual void post_update() override;
		BANANA_API virtual void pre_render() override;
		BANANA_API virtual void render() override;
		BANANA_API virtual void post_render() override;
		BANANA_API virtual void on_collision(gameobject* other) override;
		BANANA_API virtual void clean_up() override;
	private:
		void(*m_destroy_script)(native_script_component*);
		script* m_script;
	};
	template<typename T> inline void native_script_component::bind() {
		if (this->m_destroy_script) this->m_destroy_script(this);
		this->m_script = (script*)new T(this->parent, this);
		this->m_script->initialize();
		this->m_destroy_script = [](native_script_component* nsc) { delete nsc->get_script<T>(); };
		this->get_property<property_base::read_only_text>("Script")->get_text() = typeid(T).name();
	}
	template<typename T> inline void native_script_component::bind(T* s) {
		if (this->m_destroy_script) this->m_destroy_script(this);
		this->m_script = (script*)s;
		this->m_destroy_script = [](native_script_component* nsc) { delete nsc->get_script<T>(); };
		this->get_property<property_base::read_only_text>("Script")->get_text() = typeid(T).name();
	}
	template<typename T> inline T* native_script_component::get_script() {
		return (T*)this->m_script;
	}
	template<typename _Ty> inline void script::set_property(const std::string& name, const _Ty& value) {
		this->interface->set_property<_Ty>(name, value);
	}
	template<typename _Ty> inline _Ty* script::get_property(const std::string& name) {
		return this->interface->get_property<_Ty>(name);
	}
	template<typename _Ty> inline component::property<_Ty>* script::find_property(const std::string& name) {
		return this->interface->find_property<_Ty>(name);
	}
	template<typename _Ty> inline _Ty& script::add_component() {
		return this->parent->add_component<_Ty>();
	}
	template<typename _Ty> inline size_t script::get_number_components() {
		return this->parent->get_number_components<_Ty>();
	}
	template<typename _Ty> inline bool script::has_component() {
		return this->parent->has_component<_Ty>();
	}
	template<typename _Ty> inline _Ty& script::get_component(size_t index) {
		return this->parent->get_component<_Ty>(index);
	}
	template<typename _Ty> inline void script::remove_component(size_t index) {
		this->parent->remove_component<_Ty>(index);
	}
}