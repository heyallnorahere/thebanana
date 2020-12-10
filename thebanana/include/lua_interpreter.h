#pragma once
#include "banana_api.h"
namespace thebanana {
	class lua_interpreter {
	public:
#ifndef BANANA_BUILD
		struct lua_State;
#endif
		using func = int(*)(lua_State*);
		enum class type {
			_char,
			_short,
			_int,
			_long_long,
			ptr,
		};
		struct param_type {
			type _type;
			bool _unsigned;
		};
		BANANA_API lua_interpreter();
		BANANA_API ~lua_interpreter();
		BANANA_API void open_file(const std::string& path);
		BANANA_API void call_function(const std::string& name, std::vector<param_type> types, ...);
		BANANA_API void register_function(const std::string& name, func ptr);
		BANANA_API static bool check_lua(int return_value);
	private:
		BANANA_API void init_state();
		BANANA_API void destroy_state();
		BANANA_API void reset_state();
		BANANA_API void add_engine_functions();
		lua_State* m_state;
		std::vector<std::string> m_registered_functions;
	};
}