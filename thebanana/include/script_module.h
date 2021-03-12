#pragma once
#include "banana_api.h"
namespace thebanana {
	class script_registry;
	class script_module {
	public:
#ifdef BANANA_WINDOWS
		using module_t = HMODULE;
#else
		using module_t = size_t;
#endif
		BANANA_API script_module(script_registry* registry, const std::string& path);
		BANANA_API ~script_module();
		BANANA_API module_t get_native_module();
	private:
		module_t m_module;
		script_registry* m_registry;
	};
}