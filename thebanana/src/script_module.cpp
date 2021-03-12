#include "pch.h"
#include "script_module.h"
#include "script_registry.h"
namespace thebanana {
	using load_proc = void(*)(script_registry*);
	script_module::script_module(script_registry* registry, const std::string& path) : m_registry(registry) {
#ifdef BANANA_WINDOWS
		this->m_module = LoadLibraryA(path.c_str());
		assert(this->m_module);
		load_proc lp = (load_proc)GetProcAddress(this->m_module, "load_scripts");
		assert(lp);
		lp(this->m_registry);
#endif
	}
	script_module::~script_module() {
#ifdef BANANA_WINDOWS
		FreeLibrary(this->m_module);
		this->m_registry->clear_script_prototypes();
#endif
	}
	script_module::module_t script_module::get_native_module() {
		return this->m_module;
	}
}