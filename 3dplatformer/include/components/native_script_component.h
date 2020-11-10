#pragma once
#include "component.h"
namespace thebanana {
	class gameobject;
	class native_script_component;
	class script {
	public:
		script(gameobject* object, native_script_component* nsc);
		virtual void initialize();
		virtual void pre_update();
		virtual void update();
		virtual void post_update();
		virtual void pre_render();
		virtual void render();
		virtual void post_render();
		virtual void on_collision(gameobject* other);
		virtual void clean_up();
	protected:
		gameobject* parent;
		native_script_component* interface;
		template<typename _Ty> void set_property(const std::string& name, const _Ty& value);
		template<typename _Ty> _Ty* get_property(const std::string& name);
		template<typename _Ty> component::property<_Ty>* find_property(const std::string& name);
		void add_property(component::property_base* p);
		transform& get_transform();
		template<typename _Ty> _Ty& add_component();
		template<typename _Ty> size_t get_number_components();
		template<typename _Ty> _Ty& get_component(size_t index = 0);
		template<typename _Ty> void remove_component(size_t index = 0);
	};
	class native_script_component : public component {
	public:
		native_script_component(gameobject* object);
		template<typename T> void bind();
		template<typename T> void bind(T* s);
		template<typename T> T* get_script();
		virtual void pre_update() override;
		virtual void update() override;
		virtual void post_update() override;
		virtual void pre_render() override;
		virtual void render() override;
		virtual void post_render() override;
		virtual void on_collision(gameobject* other) override;
		virtual void clean_up() override;
	private:
		void(*m_destroy_script)(native_script_component*);
		script* m_script;
	};
	template<typename T> inline void native_script_component::bind() {
		if (this->m_destroy_script) this->m_destroy_script(this);
		this->m_script = (script*)new T(this->parent, this);
		this->m_script->initialize();
		this->m_destroy_script = [](native_script_component* nsc) { delete nsc->get_script<T>(); };
		this->get_property<property_base::read_only_text>("script")->get_text() = typeid(T).name();
	}
	template<typename T> inline void native_script_component::bind(T* s) {
		if (this->m_destroy_script) this->m_destroy_script(this);
		this->m_script = (script*)s;
		this->m_destroy_script = [](native_script_component* nsc) { delete nsc->get_script<T>(); };
		this->get_property<property_base::read_only_text>("script")->get_text() = typeid(T).name();
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
	template<typename _Ty> inline _Ty& script::get_component(size_t index) {
		return this->parent->get_component<_Ty>(index);
	}
	template<typename _Ty> inline void script::remove_component(size_t index) {
		this->parent->remove_component<_Ty>(index);
	}
}