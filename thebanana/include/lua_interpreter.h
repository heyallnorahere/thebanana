#pragma once
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
		lua_interpreter();
		~lua_interpreter();
		void open_file(const std::string& path);
		void call_function(const std::string& name, std::vector<param_type> types, ...);
		void register_function(const std::string& name, func ptr);
		static bool check_lua(int return_value);
	private:
		void init_state();
		void destroy_state();
		void reset_state();
		void add_engine_functions();
		lua_State* m_state;
		std::vector<std::string> m_registered_functions;
	};
}