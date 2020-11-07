#pragma once
#include "transform.h"
#include "debug_tools.h"
namespace thebanana {
	class gameobject;
	class component {
	public:
		class property_base {
		public:
			class dropdown {
			public:
				dropdown(const std::vector<std::string>& items, int initial_index = 0);
				dropdown(const std::vector<const char*>& items, int initial_index = 0);
				int* get_index_ptr();
				const std::vector<std::string>& get_items() const;
				void set_items(const std::vector<std::string>& items);
			private:
				std::vector<std::string> m_items;
				int m_index;
			};
			class read_only_text {
			public:
				read_only_text(const std::string& text);
				std::string& get_text();
			private:
				std::string text;
			};
			property_base(const std::string& name, size_t size);
			const std::string& get_name();
			virtual ~property_base();
			void* get_ptr();
			virtual void draw() const = 0;
			bool is_selection_window_open() const;
			virtual void close_selection_window() = 0;
			virtual gameobject** get_selection_window_ptr() const = 0;
		protected:
			std::string name;
			void* ptr;
			bool selection_window_open;
			gameobject* selection_window_temp;
		};
		using properties_t = std::vector<std::unique_ptr<property_base>>;
		template<typename T> class property : public property_base {
		public:
			property(const T& value, const std::string& name);
			property(const property<T>& other);
			const property<T>& operator=(const property<T>& other);
			virtual ~property() override;
			virtual void draw() const override;
			T* get_value();
			virtual void close_selection_window() override;
			virtual gameobject** get_selection_window_ptr() const override;
		private:
			T* value;
		};
		component(gameobject* obj);
		// runs either when the parent gameobject is added to a scene/another gameobject or when the component is added to a gameobject; do not add other components during this stage, do it in constructor instead
		virtual void initialize();
		// runs before update
		virtual void pre_update();
		// runs as part of the update
		virtual void update();
		// runs after update
		virtual void post_update();
		// runs before render
		virtual void pre_render();
		// runs as part of the parent gameobject's render
		virtual void render();
		// runs after render
		virtual void post_render();
		// runs when gameobject collides with another
		virtual void on_collision(gameobject* other);
		// runs on gameobject destruction
		virtual void clean_up();
		const properties_t& get_properties() const;
		gameobject* get_parent();
		template<typename _Ty> void set_property(const std::string& name, const _Ty& value);
		template<typename _Ty> _Ty* get_property(const std::string& name);
	protected:
		void add_property(property_base* p);
		transform& get_transform();
		template<typename _Ty> size_t get_number_components();
		template<typename _Ty> _Ty& get_component(size_t index = 0);
		template<typename _Ty> property<_Ty>* find_property(const std::string& name);
		properties_t properties;
		gameobject* parent;
		friend class script;
	};
	class debug_component : public component {
	public:
		debug_component(gameobject* obj);
		virtual void pre_render() override;
	private:
		double flash_start_time, flash_end_time;
	};
	template<typename T> inline component::property<T>::property(const T& value, const std::string& name) : property_base(name, sizeof(T)) {
		this->value = new(this->ptr) T(value);
	}
	template<typename T> inline component::property<T>::property(const property<T>& other) : property(*other.value, other.name) { }
	template<typename T> inline const component::property<T>& component::property<T>::operator=(const property<T>& other) {
		*this->value = *other.value;
		this->name = other.name;
	}
	template<typename T> inline component::property<T>::~property() {
		this->value->~T();
		free(this->ptr);
	}
	template<typename T> inline T* component::property<T>::get_value() {
		return this->value;
	}
	inline gameobject** component::property<gameobject*>::get_selection_window_ptr() const {
		return &((gameobject * &)this->selection_window_temp);
	}
	template<typename T> inline gameobject** component::property<T>::get_selection_window_ptr() const {
		return NULL;
	}
#ifdef BANANA_BUILD
	inline void component::property<int>::draw() const {
		ImGui::InputInt(this->name.c_str(), this->value);
	}
	inline void component::property<bool>::draw() const {
		ImGui::Checkbox(this->name.c_str(), this->value);
	}
	inline void component::property<float>::draw() const {
		ImGui::InputFloat(this->name.c_str(), this->value);
	}
	inline void component::property<double>::draw() const {
		ImGui::InputDouble(this->name.c_str(), this->value);
	}
	inline void component::property<std::string>::draw() const {
		ImGui::InputText(this->name.c_str(), this->value);
	}
	inline void component::property<glm::vec2>::draw() const {
		ImGui::InputFloat2(this->name.c_str(), &this->value->x);
	}
	inline void component::property<glm::vec3>::draw() const {
		ImGui::InputFloat3(this->name.c_str(), &this->value->x);
	}
	inline void component::property<glm::vec4>::draw() const {
		ImGui::InputFloat4(this->name.c_str(), &this->value->x);
	}
	inline void component::property<component::property_base::dropdown>::draw() const {
		const std::vector<std::string>& std_items = this->value->get_items();
		std::vector<const char*> items;
		for (auto& str : std_items) {
			items.push_back(str.c_str());
		}
		ImGui::Combo(this->name.c_str(), this->value->get_index_ptr(), items.data(), items.size());
	}
	inline void component::property<component::property_base::read_only_text>::draw() const {
		ImGui::InputText(this->name.c_str(), &this->value->get_text(), ImGuiInputTextFlags_ReadOnly);
	}
	template<typename T> inline void component::property<T>::draw() const {
		std::string name = typeid(T).name();
		size_t pos = name.find_last_of('*');
		if (pos != std::string::npos) {
			std::string substr = name.substr(pos, 7);
			if (substr == "* __ptr") {
				std::string text;
				std::string ptr_type = name.substr(0, pos - 1);
				if (*this->value) {
					text = ptr_type;
				}
				else {
					text = "null (" + ptr_type + ")";
				}
				ImGui::InputText(this->name.c_str(), &text, ImGuiInputTextFlags_ReadOnly);
				ImGui::SameLine();
				if (ImGui::Button("clear")) {
					*this->value = NULL;
				}
				if (typeid(T).hash_code() == typeid(gameobject*).hash_code()) {
					if (ImGui::Button("open gameobject selection window")) {
						property<T>* p = (property<T>*)this;
						p->selection_window_open = true;
					}
				}
				return;
			}
		}
		ImGui::Text("sorry, no implementation for this type yet... heres the raw memory though");
		ImGui::InputText(this->name.c_str(), (char*)this->ptr, sizeof(T));
	}
#endif
	template<typename T> inline void component::property<T>::close_selection_window() {
		this->selection_window_open = false;
	}
	inline void component::property<gameobject*>::close_selection_window() {
		this->selection_window_open = false;
		*this->value = this->selection_window_temp;
	}
	template<typename _Ty> inline void component::set_property(const std::string& name, const _Ty& value) {
		property<_Ty>* prop = this->find_property<_Ty>(name);
		if (prop) {
			*prop->get_value() = value;
		}
	}
	template<typename _Ty> inline _Ty* component::get_property(const std::string& name) {
		property<_Ty>* prop = this->find_property<_Ty>(name);
		if (prop) {
			return prop->get_value();
		}
		return NULL;
	}
	template<typename _Ty> inline size_t component::get_number_components() {
		return this->parent->get_number_components<_Ty>();
	}
	template<typename _Ty> inline _Ty& component::get_component(size_t index) {
		return this->parent->get_component<_Ty>(index);
	}
	template<typename _Ty> inline component::property<_Ty>* component::find_property(const std::string& name) {
		property<_Ty>* result = NULL;
		for (auto& p : this->properties) {
			if (p->get_name() == name) {
				result = (property<_Ty>*)p.get();
				break;
			}
		}
		return result;
	}
}