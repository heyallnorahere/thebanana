#pragma once
#include "components/native_script_component.h"
namespace thebanana {
	class game;
	class script_registry {
	public:
		script_registry(game* g_game);
		game* get_game();
		template<typename T> void register_script();
		script* create_script(const std::string& type_name, gameobject* parent, native_script_component* nsc);
		void clear_script_prototypes();
		std::vector<std::string> get_script_names();
	private:
		std::map<std::string, script*(*)(gameobject*, native_script_component*)> m_script_prototypes;
		game* m_game;
		template<typename T> static script* create(gameobject* parent, native_script_component* nsc);
	};
	inline script_registry::script_registry(game* g_game) {
		this->m_game = g_game;
	}
	inline game* script_registry::get_game() {
		return this->m_game;
	}
	template<typename T> inline void script_registry::register_script() {
		this->m_script_prototypes[typeid(T).name()] = create<T>;
	}
	inline script* script_registry::create_script(const std::string& type_name, gameobject* parent, native_script_component* nsc) {
		return this->m_script_prototypes[type_name](parent, nsc);
	}
	template<typename T> inline script* script_registry::create(gameobject* parent, native_script_component* nsc) {
		return new T(parent, nsc);
	}
	inline void script_registry::clear_script_prototypes() {
		this->m_script_prototypes.clear();
	}
	inline std::vector<std::string> script_registry::get_script_names() {
		std::vector<std::string> names;
		for (auto p : this->m_script_prototypes) {
			names.push_back(p.first);
		}
		return names;
	}
}