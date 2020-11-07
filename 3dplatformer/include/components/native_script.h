#pragma once
#include "component.h"
namespace thebanana {
	class gameobject;
	class native_script_component;
	class script {
	public:
		script(gameobject* object);
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
	};
	class native_script_component : public component {
	public:
		native_script_component(gameobject* object);
		template<typename T> void bind();
		script* get_script();
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
		this->m_script = (script*)new T(this->parent);
		this->m_destroy_script = [](native_script_component* x) { delete (T*)(x->get_script()); };
		this->get_property<property_base::read_only_text>("script")->get_text() = typeid(T).name();
	}
}