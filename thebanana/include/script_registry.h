#pragma once
#include "components/native_script_component.h"
namespace thebanana {
	class script_registry {
	public:
		template<typename T> void register_script();
		script* create_script(const std::string& type_name, gameobject* parent, native_script_component* nsc);
		void clear_script_prototypes();
	private:
		std::map<std::string, script*(*)(gameobject*, native_script_component*)> m_script_prototypes;
		template<typename T> static script* create(gameobject* parent, native_script_component* nsc);
	};
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
}