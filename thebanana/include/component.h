#pragma once
#include "transform.h"
#include "debug_tools.h"
#include "scene_serializer.h"
#include "game.h"
#include "banana_api.h"
namespace thebanana {
	class gameobject;
	class material;
	class component {
	public:
		class property_base {
		public:
			class dropdown {
			public:
				BANANA_API dropdown(const std::vector<std::string>& items, int initial_index = 0);
				BANANA_API dropdown(const std::vector<const char*>& items, int initial_index = 0);
				BANANA_API int* get_index_ptr();
				BANANA_API void set_item(int index);
				BANANA_API void set_item(const std::string& value);
				BANANA_API const std::vector<std::string>& get_items() const;
				BANANA_API void set_items(const std::vector<std::string>& items);
			private:
				std::vector<std::string> m_items;
				int m_index;
			};
			class read_only_text {
			public:
				BANANA_API read_only_text(const std::string& text);
				BANANA_API std::string& get_text();
			private:
				std::string text;
			};
			class color {
			public:
				BANANA_API color(glm::vec3 vector);
				BANANA_API const color& operator=(glm::vec3 vector);
				BANANA_API glm::vec3& get_vector();
			private:
				glm::vec3 m_vector;
			};
			BANANA_API property_base(const std::string& name, size_t size);
			BANANA_API void set_parent(component* parent);
			BANANA_API void set_game(game* g_game);
			BANANA_API const std::string& get_name();
			BANANA_API virtual ~property_base();
			BANANA_API void* get_ptr();
			BANANA_API virtual void draw() const = 0;
#ifdef BANANA_BUILD
			using emitter = YAML::Emitter &;
#else
			using emitter = void*;
#endif
			BANANA_API virtual void send_to_yaml(void* out) const = 0;
			BANANA_API virtual std::string get_type_name() const = 0;
			BANANA_API bool is_selection_window_open() const;
			BANANA_API virtual void close_selection_window() = 0;
			BANANA_API virtual gameobject** get_selection_window_ptr() const = 0;
		protected:
			std::string name;
			void* ptr;
			bool selection_window_open;
			gameobject* selection_window_temp;
			component* parent;
			game* g_game;
			friend class scene_serializer;
		};
		using properties_t = std::list<std::unique_ptr<property_base>>;
		template<typename T> class property : public property_base {
		public:
			property(const T& value, const std::string& name);
			property(const property<T>& other);
			const property<T>& operator=(const property<T>& other);
			virtual ~property() override;
			BANANA_API virtual void draw() const override;
			BANANA_API virtual void send_to_yaml(void* out) const override;
			virtual std::string get_type_name() const override;
			T* get_value();
			virtual void close_selection_window() override;
			virtual gameobject** get_selection_window_ptr() const override;
		private:
			T* value;
		};
		BANANA_API component(gameobject* obj);
		// runs either when the parent gameobject is added to a scene/another gameobject or when the component is added to a gameobject; do not add other components during this stage, do it in constructor instead
		BANANA_API virtual void initialize();
		// runs before update
		BANANA_API virtual void pre_update();
		// runs as part of the update
		BANANA_API virtual void update();
		// runs after update
		BANANA_API virtual void post_update();
		// runs before render
		BANANA_API virtual void pre_render();
		// runs as part of the parent gameobject's render
		BANANA_API virtual void render();
		// runs after render
		BANANA_API virtual void post_render();
		// runs when gameobject collides with another
		BANANA_API virtual void on_collision(gameobject* other);
		// runs on gameobject destruction
		BANANA_API virtual void clean_up();
		BANANA_API const properties_t& get_properties() const;
		BANANA_API gameobject* get_parent();
		template<typename _Ty> void set_property(const std::string& name, const _Ty& value);
		template<typename _Ty> _Ty* get_property(const std::string& name);
		BANANA_API unsigned long long get_uuid() const;
		BANANA_API void set_uuid(unsigned long long uuid);
		BANANA_API void add_property(property_base* p);
		BANANA_API void remove_property(const std::string& name);
	protected:
		BANANA_API transform& get_transform();
		template<typename _Ty> bool has_component();
		template<typename _Ty> size_t get_number_components();
		template<typename _Ty> _Ty& get_component(size_t index = 0);
		template<typename _Ty> property<_Ty>* find_property(const std::string& name);
		properties_t properties;
		gameobject* parent;
		unsigned long long uuid;
		friend class script;
	};
	namespace property_classes {
		using dropdown = component::property_base::dropdown;
		using read_only_text = component::property_base::read_only_text;
		using color = component::property_base::color;
	}
	class debug_component : public component {
	public:
		BANANA_API debug_component(gameobject* obj);
		BANANA_API virtual void pre_render() override;
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
	template<typename T> inline std::string component::property<T>::get_type_name() const {
		return typeid(T).name();
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
		this->g_game->get_imgui_pointer<int>()(this->name.c_str(), this->value);
	}
	inline void component::property<bool>::draw() const {
		this->g_game->get_imgui_pointer<bool>()(this->name.c_str(), this->value);
	}
	inline void component::property<float>::draw() const {
		this->g_game->get_imgui_pointer<float>()(this->name.c_str(), this->value);
	}
	inline void component::property<double>::draw() const {
		this->g_game->get_imgui_pointer<double>()(this->name.c_str(), this->value);
	}
	inline void component::property<std::string>::draw() const {
		this->g_game->get_imgui_pointer<std::string>()(this->name.c_str(), this->value);
	}
	inline void component::property<glm::vec2>::draw() const {
		this->g_game->get_imgui_pointer<glm::vec2>()(this->name.c_str(), this->value);
	}
	inline void component::property<glm::vec3>::draw() const {
		this->g_game->get_imgui_pointer<glm::vec3>()(this->name.c_str(), this->value);
	}
	inline void component::property<glm::vec4>::draw() const {
		this->g_game->get_imgui_pointer<glm::vec4>()(this->name.c_str(), this->value);
	}
	inline void component::property<component::property_base::dropdown>::draw() const {
		this->g_game->get_imgui_pointer<dropdown>()(this->name.c_str(), this->value);
	}
	inline void component::property<component::property_base::read_only_text>::draw() const {
		this->g_game->get_imgui_pointer<read_only_text>()(this->name.c_str(), this->value);
	}
	inline void component::property<property_classes::color>::draw() const {
		this->g_game->get_imgui_pointer<color>()(this->name.c_str(), this->value);
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
	inline void component::property<component::property_base::read_only_text>::send_to_yaml(void* out) const {
		(*(YAML::Emitter*)out) << this->value->get_text();
	}
	inline void component::property<component::property_base::dropdown>::send_to_yaml(void* out) const {
		YAML::Emitter& _out = (*(YAML::Emitter*)out);
		_out << YAML::BeginMap;
		_out << YAML::Key << "index" << YAML::Value << *this->value->get_index_ptr();
		_out << YAML::Key << "items" << YAML::Value << YAML::BeginSeq;
		for (auto& item : this->value->get_items()) {
			_out << item;
		}
		_out << YAML::EndSeq;
		_out << YAML::EndMap;
	}
	unsigned long long get_uuid(gameobject* obj);
	unsigned long long get_uuid(material* mat);
	inline void component::property<gameobject*>::send_to_yaml(void* out) const {
		(*(YAML::Emitter*)out) << ((*this->value) ? ::thebanana::get_uuid(*this->value) : 0);
	}
	inline void component::property<material*>::send_to_yaml(void* out) const {
		(*(YAML::Emitter*)out) << ((*this->value) ? ::thebanana::get_uuid(*this->value) : 0);
	}
	inline void component::property<component::property_base::color>::send_to_yaml(void* out) const {
		(*(YAML::Emitter*)out) << this->value->get_vector();
	}
	template<typename T> inline void component::property<T>::send_to_yaml(void* out) const {
		(*(YAML::Emitter*)out) << (*this->value);
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