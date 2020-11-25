#pragma once
namespace thebanana {
	class script_registry;
	class script_module {
	public:
#ifdef _WIN32
		using module_t = HMODULE;
#endif
		script_module(script_registry* registry, const std::string& path);
		~script_module();
		module_t get_native_module();
	private:
		module_t m_module;
		script_registry* m_registry;
	};
}